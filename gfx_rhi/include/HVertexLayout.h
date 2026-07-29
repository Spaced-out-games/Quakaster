#pragma once

#include "VertexLayoutDesc.h"
#include <Resource.h>

namespace qk::gfx::rhi {

    struct VertexLayout_impl;

    using HVertexLayout = VertexLayout_impl*;

    HVertexLayout VertexLayout_ctor(const VertexLayout_recipe& desc);

    void VertexLayout_dtor(const HVertexLayout& hLayout);

    void VertexLayout_free(HVertexLayout& hLayout);

    bool VertexLayout_valid(
        const HVertexLayout& hLayout);

    using VertexLayoutRegistry = ResourceRegistry<VertexLayout_impl, VertexLayout_dtor, VertexLayout_free>;

    Resource* Resource_cast(const HVertexLayout& hBuffer);




}