#include "DrawCommand.h"

namespace qk::gfx::rhi {
    DrawCmd::DrawCmd(BindPipelineCmd bindPipelineCmd): bindPipelineCmd(bindPipelineCmd), type(EDrawCmdType::BindPipeline) {

    }
    DrawCmd::DrawCmd(BindVertexBufferCmd bindVertexBufferCmd) : bindVertexBufferCmd(bindVertexBufferCmd), type(EDrawCmdType::BindVertexBuffer) {

    }
    DrawCmd::DrawCmd(BindIndexBufferCmd bindIndexBufferCmd) : bindIndexBufferCmd(bindIndexBufferCmd), type(EDrawCmdType::BindIndexBuffer) {

    }
    DrawCmd::DrawCmd(DrawArraysCmd drawArraysCmd) : drawArraysCmd(drawArraysCmd), type(EDrawCmdType::DrawArrays) {

    }
}