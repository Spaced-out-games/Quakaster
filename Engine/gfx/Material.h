/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/Material.h
/// Purpose: Defines a graphical material type, around the rhi.
/// **************************************************************************************************
#pragma once
#include <HDescriptorSet.h>
#include <HShaderProgram.h>
#include <Result.h>
#include <string>
#include <glm/glm.hpp>

using namespace qk::gfx::rhi;


namespace qk::gfx {


	template <typename T>
	struct MaterialTraits;



    template <>
    struct MaterialTraits<float> {
        static constexpr ScalarType type = ScalarType::f32;
        static constexpr ScalarMultiplicity mult = ScalarMultiplicity::Scalar;
        static constexpr uint16_t count = 1;
    };

    template <>
    struct MaterialTraits<glm::vec2> {
        static constexpr ScalarType type = ScalarType::f32;
        static constexpr ScalarMultiplicity mult = ScalarMultiplicity::Vector;
        static constexpr uint16_t count = 2;
    };

    template <>
    struct MaterialTraits<glm::vec3> {
        static constexpr ScalarType type = ScalarType::f32;
        static constexpr ScalarMultiplicity mult = ScalarMultiplicity::Vector;
        static constexpr uint16_t count = 3;
    };

    template <>
    struct MaterialTraits<glm::vec4> {
        static constexpr ScalarType type = ScalarType::f32;
        static constexpr ScalarMultiplicity mult = ScalarMultiplicity::Vector;
        static constexpr uint16_t count = 4;
    };

    template <>
    struct MaterialTraits<int> {
        static constexpr ScalarType type = ScalarType::i32;
        static constexpr ScalarMultiplicity mult = ScalarMultiplicity::Scalar;
        static constexpr uint16_t count = 1;
    };






	struct Material {
		HShaderProgram m_hShaderProgram;
		HDescriptorSet m_hDescriptorSet;

		int init(const std::string& vertSrc, const std::string& fragSrc);



		void bind() const;

		void update() const;

        template <class T>
        void pushUniform(const std::string& name, T* data)
        {
            auto result = rhi::ShaderProgram_getUniform(m_hShaderProgram, name.c_str());///* your lookup by name */;
            if (!result.value().valid()) return;
            //if (!result.has_error()) return;

            //HShaderUniform gl_ShaderLocation(result.value());
            using Traits = MaterialTraits<T>;
            HShaderUniform gl_ShaderLocation = result.value();
            rhi::DescriptorSet_push(
                m_hDescriptorSet,
                gl_ShaderLocation,
                Traits::type,
                Traits::count,
                Traits::mult,
                data
            );
        }

	};
}