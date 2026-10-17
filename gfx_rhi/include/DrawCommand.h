#pragma once
// We have to forward-declare the pipeline stuff to prevent circular dependencies.
// #include <HPipeline.h>

#include <core.h>
#include <vector>
#include <ostream>

// NOTICE: These are meant to be draw command descriptions! 

namespace qk::gfx::rhi {

    // Pipeline implementation
    struct Pipeline_impl;

    // Pipeline handle
    using HPipeline = Pipeline_impl*;

    
    struct Buffer_impl;

    struct DescriptorSet_impl;

    using HVertexBuffer = Buffer_impl*;
    using HIndexBuffer = Buffer_impl*;
    using HDescriptorSet = DescriptorSet_impl*;



    enum class EDrawCmdType {
        BindPipeline,
        BindVertexBuffer,
        BindIndexBuffer,
        BindDescriptorSet,
        DrawArrays,
        DrawArraysInstanced // not implemented yet.
    };

    struct BindPipelineCmd {
        HPipeline hPipeline = nullptr;
    };
    
    struct BindVertexBufferCmd {
        HVertexBuffer hVertexBuffer = nullptr;
        u32 offset = 0;
        u32 stride = 0;
        u8 slot = 0;
    };
    
    struct BindIndexBufferCmd {
        HIndexBuffer hIndexBuffer = nullptr;
    };

    struct DrawArraysCmd {
        u32 offset = 0;
        u32 count = 0;
    };

    struct DrawArraysInstancedCmd {
        u32 offset = 0;
        u32 numVertices = 0;
        u32 numInstances = 0;
    };

    struct BindDescriptorSetCmd {
        HDescriptorSet set;
        u8 slot = 0;
    };
    


    struct DrawCmd {
        EDrawCmdType type;

        union {
            BindPipelineCmd     bindPipelineCmd;
            BindDescriptorSetCmd bindDescriptorSetCmd;
            BindVertexBufferCmd bindVertexBufferCmd;
            BindIndexBufferCmd  bindIndexBufferCmd;
            DrawArraysCmd       drawArraysCmd;
            DrawArraysInstancedCmd drawArraysInstancedCmd;
        };

        DrawCmd(BindPipelineCmd bindPipelineCmd);
        DrawCmd(BindDescriptorSetCmd BindDescriptorSetCmd);
        DrawCmd(BindVertexBufferCmd bindVertexBufferCmd);
        DrawCmd(BindIndexBufferCmd bindIndexBufferCmd);
        DrawCmd(DrawArraysCmd drawArraysCmd);
        DrawCmd(DrawArraysInstancedCmd drawArraysInstancedCmd);

    };


    // debug
    inline std::ostream& operator<<(std::ostream& os, const DrawCmd& cmd)
    {
        switch (cmd.type) {
        case EDrawCmdType::BindPipeline:
            return os << "BindPipeline { "
                << "pipeline=" << cmd.bindPipelineCmd.hPipeline
                << " }";

        case EDrawCmdType::BindVertexBuffer: {
            const auto& c = cmd.bindVertexBufferCmd;
            return os << "BindVertexBuffer { "
                << "buffer=" << c.hVertexBuffer
                << ", offset=" << c.offset
                << ", stride=" << c.stride
                << ", slot=" << static_cast<unsigned>(c.slot)
                << " }";
        }

        case EDrawCmdType::BindIndexBuffer:
            return os << "BindIndexBuffer { "
                << "buffer=" << cmd.bindIndexBufferCmd.hIndexBuffer
                << " }";

        case EDrawCmdType::BindDescriptorSet: {
            const auto& c = cmd.bindDescriptorSetCmd;
            return os << "BindDescriptorSet { "
                << "set=" << c.set
                << ", slot=" << static_cast<unsigned>(c.slot)
                << " }";
        }

        case EDrawCmdType::DrawArrays: {
            const auto& c = cmd.drawArraysCmd;
            return os << "DrawArrays { "
                << "offset=" << c.offset
                << ", count=" << c.count
                << " }";
        }

        case EDrawCmdType::DrawArraysInstanced: {
            const auto& c = cmd.drawArraysInstancedCmd;
            return os << "DrawArraysInstanced { "
                << "offset=" << c.offset
                << ", numVertices=" << c.numVertices
                << ", numInstances=" << c.numInstances
                << " }";
        }
        }

        return os << "DrawCmd { unknown command type="
            << static_cast<int>(cmd.type) << " }";
    }



    struct DescriptorSetBinding {
        HDescriptorSet hDescriptorSet = nullptr;
        u8 slot = 0;
    };

    using DescriptorSetBindings = std::vector<DescriptorSetBinding>;


}