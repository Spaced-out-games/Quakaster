#pragma once

#include "VertexLayoutDesc.h"
#include <Resource.h>

namespace qk::gfx::rhi {

    struct VertexLayout_impl;

    using RVertexLayout = VertexLayout_impl*;

    RVertexLayout VertexLayout_ctor(
        str_view label,
        const VertexLayout_recipe& desc);

    bool VertexLayout_dtor(
        RVertexLayout& hLayout);

    bool VertexLayout_valid(
        const RVertexLayout& hLayout);

    // NOTICE: This assumes a VertexArray has already been bound.
    bool VertexLayout_apply(
        const RVertexLayout& hLayout);

}