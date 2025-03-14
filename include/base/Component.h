// QKComponent.h
// PURPOSE: Create a base QKComponent type compatable with EnTT.

#pragma once
#include <include/thirdparty/entt.hpp>



struct QKEntity; // forward declaration


/// <summary>
/// Base component type written over EnTT. 
/// It represents a logical set of data and functions on that data, 
/// There's not much purpose to this except explicitness
/// </summary>
struct QKComponent {
            
        QKComponent() = default;
        virtual ~QKComponent() = default;
};

