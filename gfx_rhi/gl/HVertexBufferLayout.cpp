#include "HVertexBufferLayout.h"
#include "HVertexArray.h"
#include <vector>

using qk::gfx::rhi::HVertexBufferLayoutTag;


namespace qk::gfx::rhi {
	using VertexBufferLayout = VertexBufferLayoutDesc;
}

using qk::gfx::rhi::VertexBufferLayout;


namespace qk {
	
	template<>
	inline VertexBufferLayout* get<HVertexBufferLayoutTag, VertexBufferLayout*>(const Handle<HVertexBufferLayoutTag>& handle) {
		return (VertexBufferLayout*)handle.m_Handle;
	}

	template<>
	inline void set<HVertexBufferLayoutTag, VertexBufferLayout*>(Handle<HVertexBufferLayoutTag>& handle, VertexBufferLayout* value) {
		handle.m_Handle = (uintptr_t)value;
	}


}

namespace qk::gfx::rhi {

	


	HVertexBufferLayout VertexBufferLayout_ctor(const VertexBufferLayoutDesc& desc) {
		HVertexBufferLayout result;
		auto* layout = new VertexBufferLayout();
		*layout = desc;

		// we should also build the bindings and shit here.

		set<HVertexBufferLayoutTag, VertexBufferLayout*>(result, layout);
		return result;
	}
	void VertexBufferLayout_dtor(HVertexBufferLayout& hLayout) {
		
		auto* layout = get<HVertexBufferLayoutTag, VertexBufferLayout*>(hLayout);

		if (layout) delete layout;

		set<HVertexBufferLayoutTag, VertexBufferLayout*>(hLayout, nullptr);
		
	}

}