/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/LayerStack.h
/// Purpose: Defines an interface for layered event propagation
/// ************************************************************************************************** 

#pragma once
#include "core.h"
#include "Layer.h"
#include "EventQueue.h"
#include <memory>
#include <vector>
#include <type_traits>


namespace qk {
    class QK_API LayerStack {
        std::vector<std::unique_ptr<Layer>> m_Layers;
        EventQueue* m_pQueue = nullptr;

    public:

        void attachQueue(EventQueue* pQueue);

        
        void insertLayer(std::unique_ptr<Layer>&& pLayer);


        template<typename layer_t, typename... Args>
        layer_t& emplace_layer(Args&&... args) {
            static_assert(std::is_base_of_v<Layer, layer_t>, "layer_t must derive from Layer");

            auto pLayer = std::make_unique<layer_t>(std::forward<Args>(args)...);

            pLayer->onAttach();

            m_Layers.push_back(std::move(pLayer));

            return *(layer_t*)(m_Layers.back().get());
        }


        // Delete copy operations
        LayerStack(const LayerStack&) = delete;
        LayerStack& operator=(const LayerStack&) = delete;

        // Default move operations
        LayerStack(LayerStack&&) noexcept = default;
        LayerStack& operator=(LayerStack&&) noexcept = default;
        LayerStack() = default;

        void propagate_events();

        void render();

        Layer* operator[](size_t index) {
            if (index >= m_Layers.size()) return nullptr;
            return m_Layers[index].get();
        }


    };
}

