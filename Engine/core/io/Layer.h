/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/Layer.c
/// Purpose: Defines an interface for layered event propagation
/// ************************************************************************************************** 

#pragma once
#include "Event.h"
#include "../../core.h"


namespace qk::io {
	struct QK_API Layer {

		EEventCategory filter = EEventCategory::cat_null;

		// Tells the layerstack what to do with the event once propagated to this layer
		enum class EBlock: uint8_t {
			Pass		= 0x00,
			Block		= 0xff
		};


		virtual ~Layer() = default;
		[[nodiscard]] virtual EBlock on_event(const Event& evt) = 0;
		virtual void on_render() = 0;
		virtual void on_attach() = 0;
		virtual void on_detach() = 0;


		void subscribe(EEventCategory category);
		void unsubscribe(EEventCategory category);
	};
}