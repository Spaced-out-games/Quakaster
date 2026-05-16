#pragma once

#include <variant>
#include <cstdint>

#include <HVertexArray.h>
#include <HVertexBuffer.h>
#include <HIndexBuffer.h>
#include <HTexture.h>
#include <HShaderProgram.h>
#include "gfx_draw.h" // this is just for PrimitiveType; eventually move it here.

namespace qk::gfx::rhi {

    // ============================================================
    // Draw Commands
    // ============================================================

    struct DrawVertexArrayCmd {
        PrimitiveType primitiveType;

        int32_t first;
        int32_t count;
    };

    struct DrawElementsCmd {

        PrimitiveType primitiveType;


        int32_t count;

        uint32_t indexType;

        uint64_t offsetBytes;
    };

    struct DrawArraysInstancedCmd {
        PrimitiveType primitiveType;


        int32_t first;
        int32_t count;

        int32_t instanceCount;
    };

    // ============================================================
    // Variant Command Type
    // ============================================================

    using DrawCommand = std::variant<
        std::monostate,
        DrawVertexArrayCmd,
        DrawElementsCmd,
        DrawArraysInstancedCmd
    >;
    void call(const DrawCommand& command);
}