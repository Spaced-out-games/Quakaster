/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/LayerStack.h
/// Purpose: Defines an interface for layered event propagation
/// ************************************************************************************************** 

#pragma once
#include "../../core.h"
#include "Layer.h"
#include "EventQueue.h"
#include <memory>
#include <vector>
#include <type_traits>


namespace qk::io {
    class QK_API LayerStack {
        std::vector<std::unique_ptr<Layer>> m_Layers;
        EventQueue* m_Queue = nullptr;

    public:

        void attach_queue(EventQueue* queue);

        // todo: template this!!!
        void insert_layer(std::unique_ptr<Layer>&& layer);


        template<typename layer_t, typename... Args>
        layer_t& emplace_layer(Args&&... args) {
            static_assert(std::is_base_of_v<Layer, layer_t>, "layer_t must derive from Layer");

            auto layer = std::make_unique<layer_t>(std::forward<Args>(args)...);
            layer_t& ref = *layer;

            ref.on_attach();
            m_Layers.push_back(std::move(layer));

            return ref;
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

