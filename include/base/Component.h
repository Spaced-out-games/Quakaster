// Component.h
// PURPOSE: Create a base Component type compatable with EnTT.

#pragma once
#include <include/thirdparty/entt.hpp>



namespace Quakaster::base {

    struct Entity; // forward declaration


    /// <summary>
    /// Base component type written over EnTT. 
    /// It represents a logical set of data and functions on that data, 
    /// There's not much purpose to this except explicitness
    /// </summary>
    struct Component {
            
            Component() = default;
            virtual ~Component() = default;
    };

}

using Component = Quakaster::base::Component;