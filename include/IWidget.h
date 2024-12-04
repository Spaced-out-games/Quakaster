#pragma once
#include "IWidgetComponent.h"
#include <typeindex>

using mini_type_index = uint8_t;

class IWidget {
    mini_type_index counter = 0;
    std::vector<IWidgetComponent*> components;
    std::vector<mini_type_index> type_map;
    std::unordered_map<std::type_index, mini_type_index> type_minifier;

public:
    // Template function to add components of type T
    template <typename T, typename ...Args>
    void add_component(Args&&... args) // Use perfect forwarding
    {
        // Create a new component and store it in components
        components.push_back(new T(std::forward<Args>(args)...));

        // Get the type index for the component type
        std::type_index typeIndex = std::type_index(typeid(T));

        // Check if the type index is in the map, and if not, try to add it
        if (type_minifier.find(typeIndex) == type_minifier.end()) {
            // Assign a new mini type index
            type_minifier[typeIndex] = counter++;
            type_map.push_back(type_minifier[typeIndex]);
        }
    }

    template <typename T>
    bool has_component()
    {
        // Get the type index for the requested component type
        std::type_index typeIndex = std::type_index(typeid(T));

        // Check if the type index is present in the map
        return type_minifier.find(typeIndex) != type_minifier.end();
    }

    // Optionally, you might want to add a destructor to clean up the components
    ~IWidget() {
        for (auto component : components) {
            delete component; // Assuming components are dynamically allocated
        }
    }
};