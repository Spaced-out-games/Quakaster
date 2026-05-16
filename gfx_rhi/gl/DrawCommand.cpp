#include "DrawCommand.h"
#include <glad/glad.h>

#include "rhi_ResourceDefinitions.inl"


namespace qk::gfx::rhi {

    inline GLuint PrimitiveType_cast(PrimitiveType type) {
        switch (type)
        {
        case qk::gfx::rhi::PrimitiveType::Triangle:
            return GL_TRIANGLES;
        case qk::gfx::rhi::PrimitiveType::Lines:
            return GL_LINES;
        case qk::gfx::rhi::PrimitiveType::Points:
            return GL_POINTS;
        case qk::gfx::rhi::PrimitiveType::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case qk::gfx::rhi::PrimitiveType::LineStrip:
            return GL_LINE_STRIP;
        default:
            break;
        }
    }


    // ============================================================
    // Execute
    // ============================================================

    void call(const DrawCommand& command)
    {
        std::visit([](auto&& cmd)
        {
            using T = std::decay_t<decltype(cmd)>;

        // ----------------------------------------------------
        // no-op
        // ----------------------------------------------------

        if constexpr (std::is_same_v<T, std::monostate>)
        {
            return;
        }

        // ----------------------------------------------------
        // draw
        // ----------------------------------------------------

        else if constexpr (std::is_same_v<T, DrawVertexArrayCmd>)
        {
            glDrawArrays(
                PrimitiveType_cast(cmd.primitiveType),
                cmd.first,
                cmd.count
            );
        }

        else if constexpr (std::is_same_v<T, DrawElementsCmd>)
        {
            glDrawElements(
                PrimitiveType_cast(cmd.primitiveType),
                cmd.count,
                cmd.indexType,
                reinterpret_cast<void*>(cmd.offsetBytes)
            );
        }

        else if constexpr (std::is_same_v<T, DrawArraysInstancedCmd>)
        {
            glDrawArraysInstanced(
                PrimitiveType_cast(cmd.primitiveType),
                cmd.first,
                cmd.count,
                cmd.instanceCount
            );
        }

       

        }, command);
    }

}