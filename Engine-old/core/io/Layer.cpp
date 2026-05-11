/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/Layer.cpp
/// Purpose: Defines Layer subscription logic since it's DLL-constrained
/// ************************************************************************************************** 

#include "Layer.h"

namespace qk::io {
	void Layer::subscribe(EEventCategory category) {
		filter |= category;
	}
	void Layer::unsubscribe(EEventCategory category) {
		filter &= ~category;
	}
}