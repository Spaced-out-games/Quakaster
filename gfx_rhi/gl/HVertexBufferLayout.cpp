#include "HVertexBufferLayout.h"
#include "HVertexArray.h"
#include <vector>

using qk::gfx::rhi::HVertexBufferLayoutTag;




using qk::gfx::rhi::VertexBufferLayoutDesc;



namespace qk {
	template<>
	struct ResourceTraits<HVertexBufferLayoutTag> {
		using value_type = VertexBufferLayoutDesc*;
	};

}

namespace qk::gfx::rhi {

	


	HVertexBufferLayout VertexBufferLayout_ctor(const VertexBufferLayoutDesc& desc) {
		HVertexBufferLayout result;
		auto* layout = new VertexBufferLayoutDesc();
		*layout = desc;

		// we should also build the bindings and shit here.

		set(result, layout);
		return result;
	}
	void VertexBufferLayout_dtor(HVertexBufferLayout& hLayout) {
		QK_ASSERT(hLayout.valid());

		VertexBufferLayoutDesc* pVertexBufferLayoutDesc = get(hLayout);

		if (pVertexBufferLayoutDesc) delete pVertexBufferLayoutDesc;
		hLayout = nullptr;
		
	}

}