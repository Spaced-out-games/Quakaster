#pragma once
#include "VertexBufferLayoutDesc.h"
#include "HVertexBufferLayout.h"


namespace qk::gfx {


	class VertexBufferLayout {
		rhi::HVertexBufferLayout m_hVertexBufferLayout;

		public:

			void init(const rhi::VertexBufferLayoutDesc& desc);


			void destroy();

			rhi::HVertexBufferLayoutRef ref() const;


	};

}