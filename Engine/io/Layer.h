/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/Layer.c
/// Purpose: Defines an interface for layered event propagation
/// ************************************************************************************************** 

#pragma once
#include "Event.h"
#include "core.h"


namespace qk {
	struct QK_API Layer {

		EEventCategory filter = EEventCategory::cat_null;

		// Tells the layerstack what to do with the event once propagated to this layer
		enum class EBlock: uint8_t {
			Pass		= 0x00,
			Block		= 0xff
		};


		virtual ~Layer() = default;
		[[nodiscard]] virtual EBlock onEvent(const Event& evt) = 0;
		virtual void onRender() = 0;
		virtual void onAttach() = 0;
		virtual void onDetach() = 0;


		void subscribe(EEventCategory category);
		void unsubscribe(EEventCategory category);
	};
}