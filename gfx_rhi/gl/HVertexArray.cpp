#include "HVertexArray.h"

#include "glad/glad.h"
#include "Scalar.h"

#include <Resource.h>

#include "rhi_globals.h"

namespace qk::gfx::rhi {


    HVertexArray VertexArray_ctor(str_view label)
    {
        VertexArray_impl impl;

        return g_VertexArrays.insert(
            label,
            std::move(impl));
    }

    void VertexArray_dtor(HVertexArray& hVertexArray)
    {
        if (!hVertexArray)
            return;
        

        glDeleteVertexArrays(1, &hVertexArray->vao);

        g_VertexArrays.destroy(hVertexArray);
        hVertexArray = nullptr;
    }

    bool VertexArray_valid(const HVertexArray& hVertexArray)
    {
        return g_VertexArrays.valid(hVertexArray);
    }

    bool VertexArray_bind(const HVertexArray& hVertexArray)
    {
        if (!hVertexArray)
            return false;

        if (!g_VertexArrays.valid(hVertexArray))
            return false;

        glBindVertexArray(hVertexArray->vao);
        return true;
    }

}