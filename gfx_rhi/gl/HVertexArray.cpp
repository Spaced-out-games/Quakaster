#include "HVertexArray.h"

#include "glad/glad.h"
#include "Scalar.h"

#include <Resource.h>

#include "rhi_globals.h"

namespace qk::gfx::rhi {


    struct VertexArray_impl : qk::Resource
    {
        GLuint vao = 0;

        VertexArray_impl()
        {
            glGenVertexArrays(1, &vao);
        }

        bool destroy() override {
            if (!ready_for_deletion() || !vao) return false;

            glDeleteVertexArrays(1, &vao);
            return true;
        }

        void bind() {
            glBindVertexArray(vao);
        }

        ~VertexArray_impl() {
            destroy();
        }

    };



    HVertexArray VertexArray_ctor(str_view label) {

        return new VertexArray_impl();

    }

    void VertexArray_dtor(HVertexArray& hVertexArray)
    {
        assert(hVertexArray);
        
        hVertexArray->destroy();

        hVertexArray = nullptr;
    }

    bool VertexArray_valid(const HVertexArray& hVertexArray)
    {
        // this isn't perfect but it works.
        return hVertexArray != nullptr;
    }

    bool VertexArray_bind(const HVertexArray& hVertexArray) {

        assert(hVertexArray);

        hVertexArray->bind();

        return true;

    }

}