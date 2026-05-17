/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/Material.h
/// Purpose: Defines a graphical material type, around the rhi.
/// **************************************************************************************************
#pragma once
#include <HDescriptorSet.h>
#include <HShaderProgram.h>
#include "Texture.h"
#include "fnv1a.h"
#include <Result.h>
#include <string>
#include <glm/glm.hpp>

using namespace qk::gfx::rhi;

using qk::string_hash;

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


    struct TextureBinding {
        Texture texture;
        rhi::HShaderUniform location;
        uint8_t slot;
    };



	struct Material {
		HShaderProgram m_hShaderProgram;
		HDescriptorSet m_hDescriptorSet;
        std::vector<TextureBinding> m_TextureBindings;
		int init(const std::string& vertSrc, const std::string& fragSrc);



		void bind() const;

		void update() const;

        template <class T>
        void pushUniform(const std::string& name, T* data)
        {
            auto result = rhi::ShaderProgram_getUniform(m_hShaderProgram, name.c_str());///* your lookup by name */;
            if (!result.valid()) return;
            //if (!result.has_error()) return;

            //HShaderUniform gl_ShaderLocation(result.value());
            using Traits = MaterialTraits<T>;
            rhi::DescriptorSet_push(
                m_hDescriptorSet,
                result,
                Traits::type,
                Traits::count,
                Traits::mult,
                data
            );
        }

        void pushTexture(std::string_view name, Texture&& texture);

	};
}