#pragma once

#include <rhi_Resource.h>
#include <core.h>

namespace qk::gfx::rhi {

    struct VertexArray_impl;

    using HVertexArray = VertexArray_impl*;

    HVertexArray VertexArray_ctor(str_view label);

    void VertexArray_dtor(HVertexArray& hVertexArray);

    bool VertexArray_bind(const HVertexArray& hVertexArray);

    bool VertexArray_valid(const HVertexArray& hVertexArray);

}