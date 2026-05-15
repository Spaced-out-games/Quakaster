/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/LayerStack.cpp
/// Purpose: Implements layered event propagation
/// ************************************************************************************************** 
#include "LayerStack.h"


namespace qk {

	void LayerStack::insertLayer(std::unique_ptr<Layer>&& pLayer) {
		pLayer->onAttach();
		m_Layers.push_back(std::move(pLayer));
	}

	void LayerStack::attachQueue(EventQueue* pQueue) {
		m_pQueue = pQueue;
	}



	void LayerStack::propagate_events() {

		if (!m_pQueue) return;

		Layer::EBlock BlockResult = {};


		for (const Event& evt: m_pQueue->events())
		{
			for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
			{
				auto& pLayer = *it;

				if (!(pLayer->filter & evt.m_Category)) continue;

				BlockResult = pLayer->onEvent(evt);

				if (BlockResult == Layer::EBlock::Block) {
					break;
				}
			}
		}

		m_pQueue->clear();

	}

	void LayerStack::render() {
		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it) {
			it->get()->onRender();
		}
	}


}