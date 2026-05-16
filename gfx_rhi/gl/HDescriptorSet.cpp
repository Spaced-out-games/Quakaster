#include "HDescriptorSet.h"
#include "VertexBufferLayoutDesc.h"
#include <unordered_map>
#include "glad/glad.h"


using namespace qk::gfx::rhi;

using qk::set;
using qk::get;

namespace qk::gfx::rhi {
	struct DescriptorSet;
}

namespace qk {
	template<>
	struct ResourceTraits<HDescriptorSetTag> {
		using value_type = DescriptorSet*;
	};
}

#include "rhi_ResourceDefinitions.inl"

namespace qk::gfx::rhi {


	struct Descriptor {

		void* pData = nullptr;
		ScalarDesc	scalar;
		
	};

    static void upload_uniform(GLint location, const Descriptor& desc)
    {
        const auto& s = desc.scalar;

        void* data = desc.pData;

        switch (s.type)
        {
        case ScalarType::f32:
        {
            auto* v = static_cast<float*>(data);

            if (s.multiplicity == ScalarMultiplicity::Scalar)
                glUniform1f(location, v[0]);
            else if (s.multiplicity == ScalarMultiplicity::Vector)
            {
                switch (s.count)
                {
                case 2:
                    glUniform2fv(location, 1, v);
                    break;

                case 3:
                    glUniform3fv(location, 1, v);
                    break;

                case 4:
                    glUniform4fv(location, 1, v);
                    break;

                default:
                    assert(false && "Unsupported vector dimension");
                    break;
                }
            }
            else if (s.multiplicity == ScalarMultiplicity::Matrix)
                glUniformMatrix4fv(location, s.count, GL_FALSE, v);

            break;
        }

        case ScalarType::i32:
        {
            auto* v = static_cast<int*>(data);

            if (s.multiplicity == ScalarMultiplicity::Scalar)
                glUniform1i(location, v[0]);
            else if (s.multiplicity == ScalarMultiplicity::Vector)
                glUniform1iv(location, s.count, v);

            break;
        }

        case ScalarType::u32:
        {
            auto* v = static_cast<unsigned int*>(data);

            if (s.multiplicity == ScalarMultiplicity::Scalar)
                glUniform1ui(location, v[0]);
            else if (s.multiplicity == ScalarMultiplicity::Vector)
                glUniform1uiv(location, s.count, v);

            break;
        }

        default:
            assert(false && "Unsupported ScalarType");
            break;
        }
    }


	struct DescriptorSet {
        std::unordered_map<GLint, Descriptor> m_Descriptors;
	};

	HDescriptorSet DescriptorSet_ctor() {
		HDescriptorSet hDescriptorSet;

		set(hDescriptorSet, new DescriptorSet());

		return hDescriptorSet;

	}

	void DescriptorSet_dtor(HDescriptorSet& hDescriptorSet) {

        QK_ASSERT(hDescriptorSet.valid());

		delete get(hDescriptorSet);
        hDescriptorSet = nullptr;
	}

    void DescriptorSet_push(
        HDescriptorSet& hDescriptorSet, HShaderUniform hLocation, ScalarType type,
        uint16_t scalar_count, ScalarMultiplicity multiplicity,
        void* pData) {

        QK_ASSERT(hDescriptorSet.valid());
        QK_ASSERT(hLocation.valid());

        DescriptorSet* pDescriptorSet = get(hDescriptorSet);

        pDescriptorSet->m_Descriptors.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(hLocation.value()),
            std::forward_as_tuple(pData, ScalarDesc{ multiplicity, type, scalar_count })
        );


    }

    void DescriptorSet_update(const HShaderProgram& hShaderProgram,
        const HDescriptorSet& hDescriptorSet,
        const HShaderUniform& hLocation)
    {
        QK_ASSERT(hShaderProgram.valid());
        QK_ASSERT(hDescriptorSet.valid());
        QK_ASSERT(hLocation.valid());


        GLuint gl_program = get(hShaderProgram);


        DescriptorSet* pDescriptorSet = get(hDescriptorSet);

        auto it = pDescriptorSet->m_Descriptors.find(hLocation.value());
        if (it == pDescriptorSet->m_Descriptors.end())
            return;

        glUseProgram(gl_program);

        upload_uniform(get(hLocation), it->second);
    }

    void DescriptorSet_update(const HShaderProgram& hShaderProgram,
        const HDescriptorSet& hDescriptorSet)
    {
        QK_ASSERT(hShaderProgram.valid());
        QK_ASSERT(hDescriptorSet.valid());


        GLuint gl_program = get(hShaderProgram);

        auto* pDescriptorSet = get(hDescriptorSet);

        glUseProgram(gl_program);
        HShaderUniform hCurrentUniform;
        for (const auto& [location, desc] : pDescriptorSet->m_Descriptors)
        {
            upload_uniform(location, desc);
        }
    }

}