/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/LayerStack.cpp
/// Purpose: Implements layered event propagation
/// ************************************************************************************************** 
#include "LayerStack.h"


namespace qk {

	void LayerStack::insert_layer(std::unique_ptr<Layer>&& layer) {
		layer->onAttach();
		m_Layers.push_back(std::move(layer));
	}

	void LayerStack::attach_queue(EventQueue* queue) {
		m_Queue = queue;
	}



	void LayerStack::propagate_events() {

		if (!m_Queue) return;

		Layer::EBlock BlockResult = {};


		for (const Event& evt: m_Queue->events())
		{
			for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
			{
				auto& layerPtr = *it;

				if (!(layerPtr->filter & evt.m_Category)) continue;

				BlockResult = layerPtr->onEvent(evt);

				if (BlockResult == Layer::EBlock::Block) {
					break;
				}
			}
		}

		m_Queue->clear();

	}

	void LayerStack::render() {
		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it) {
			(*it)->onRender();
		}
	}


}