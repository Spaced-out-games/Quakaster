#include "Material.h"
#include <spdlog/spdlog.h>

namespace qk::gfx {


    int Material::init(const std::string& vertSrc,
        const std::string& fragSrc)
    {
        // --- Vertex shader ---
        auto vertRes = rhi::Shader_ctor(ShaderType::Vertex);

        if (!vertRes)
        {
            spdlog::error(
                "Failed to create vertex shader: {}",
                vertRes.error()
            );

            return -1;
        }

        HShader hVertShader = vertRes.value();

        auto vertUpload = rhi::Shader_upload(hVertShader, vertSrc);

        if (!vertUpload.empty())
        {
            spdlog::error(
                "Failed to upload vertex shader: {}",
                vertUpload
            );

            return -2;
        }

        // --- Fragment shader ---
        auto fragRes = rhi::Shader_ctor(ShaderType::Fragment);

        if (!fragRes)
        {
            spdlog::error(
                "Failed to create fragment shader: {}",
                fragRes.error()
            );

            return -3;
        }

        HShader hFragShader = fragRes.value();

        auto fragUpload = rhi::Shader_upload(hFragShader, fragSrc);

        if (!fragUpload.empty())
        {
            spdlog::error(
                "Failed to upload fragment shader: {}",
                fragUpload
            );

            return -4;
        }

        // --- Program link ---
        auto programRes =
            rhi::ShaderProgram_ctor(hFragShader, hVertShader);

        if (!programRes)
        {
            spdlog::error(
                "Failed to create shader program: {}",
                programRes.error()
            );

            return -5;
        }

        m_hShaderProgram = programRes.value();

        if (!m_hDescriptorSet.valid())
        {
            m_hDescriptorSet = rhi::DescriptorSet_ctor();
        }

        spdlog::info("Material initialized successfully.");

        return 0;
    }





	void Material::bind() const {
        rhi::ShaderProgram_bind(m_hShaderProgram);
	}

	void Material::update() const {
        rhi::DescriptorSet_update(m_hShaderProgram, m_hDescriptorSet);
	};
}