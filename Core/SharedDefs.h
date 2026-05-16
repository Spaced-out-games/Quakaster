#pragma once
#include "Resource.h"
namespace qk {
	struct MonitorTag {};
	using HMonitor = Resource<MonitorTag>;

	struct WindowTag {};
	using HWindow = Resource<WindowTag>;

	using HWindowRef = Ref<WindowTag>;


	namespace gfx {
		struct HLoaderTag {};
		using HLoader = Resource<HLoaderTag>;
	}

}


