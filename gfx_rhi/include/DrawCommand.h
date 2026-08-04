#pragma once
// We have to forward-declare the pipeline stuff to prevent circular dependencies.
// #include <HPipeline.h>

#include <core.h>

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

}