#pragma once
#include "Handle.h"
namespace qk {
	struct MonitorTag {};
	using HMonitor = Handle<MonitorTag>;

	struct WindowTag {};
	using HWindow = Handle<WindowTag>;

	using HWindowRef = Ref<WindowTag>;


	namespace gfx {
		struct HLoaderTag {};
		using HLoader = Handle<HLoaderTag>;
	}

}


