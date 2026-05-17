#pragma once
#include <HVertexArray.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <span>


namespace qk::gfx {

	class VertexArray {
		rhi::HVertexArray m_hVertexArray;

		public:
			void init();

			void bind() const;
			
			void destroy();

			void apply_layout(VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

			bool valid() const;

			rhi::HVertexArrayRef ref() const;


	};


}