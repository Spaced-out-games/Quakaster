/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/VertexBuffer.h
/// Purpose: Sets up vertex array attribute pointers
/// **************************************************************************************************

#pragma once
#include "../core.h"
#include "VertexBuffer.h"
//#include "VertexArray.h"
#include <vector>
#include <cassert>
#include "types.h"
#include "GL/glew.h"
namespace qk::gfx
{





    struct VertexBufferLayoutElement {
        VertexAttribute type;
        unsigned int count;
        bool normalized;
        unsigned int divisor = 0; // 0 = per-vertex, per Nth instance otherwise
    };

    using VertexBufferLayoutCursor = unsigned int;


    unsigned int get_attribute_size(VertexAttribute attribute);

    

    

    class VertexBufferLayout {

        private:
            friend class VertexArray;
            template <class vertex_t>
            friend class VertexBuffer;
            std::vector<VertexBufferLayoutElement> m_Elements;
            unsigned int m_Stride = 0;

        public:

            template<class T>
            void push(unsigned int count, bool normalized = false, unsigned int divisor = 0)
            {
                m_Elements.emplace_back(VertexBufferLayoutElement{
                    VertexAttribute_cast<T>(),
                    count,
                    normalized,
                    divisor
                    });

                m_Stride += count * sizeof(T);
            }

            unsigned int stride() {
                return m_Stride;
            }

    };

}