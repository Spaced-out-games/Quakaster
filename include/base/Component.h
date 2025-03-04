#pragma once
//#include <include/base/Scene.h>
#include <include/thirdparty/entt.hpp>



namespace Quakaster::base {

    struct Entity; // forward declaration


    /// <summary>
    /// Base component type written over EnTT. 
    /// It represents a logical set of data and functions on that data, 
    /// as well as 'link/bridge' functions that connect two or more components 
    /// using dependency injection (e.g., controller::move(transform&, vec3)).
    /// </summary>
    struct Component {
            
            Component() = default;

            /// <summary>
            /// Virtual destructor for Component. Cleans up resources.
            /// </summary>
            virtual ~Component() = default;
    };

}

using Component = Quakaster::base::Component;