#pragma once
//#include <include/GameContext/base/Scene.h>
#include <include/thirdparty/entt.hpp>



namespace Quakaster::base {

    struct Entity;


    /// <summary>
    /// Base component type written over EnTT. 
    /// It represents a logical set of data and functions on that data, 
    /// as well as 'link/bridge' functions that connect two or more components 
    /// using dependency injection (e.g., controller::move(transform&, vec3)).
    /// </summary>
    struct Component {
    protected:
        // Grants access to Entity and Scene
        //friend struct Entity;
        //friend struct Scene;

        // Grants access to entt::registry for constructing components
        //friend class entt::registry;

        Component() {}

    public:
        /// <summary>
        /// Virtual destructor for Component. Cleans up resources.
        /// </summary>
        virtual ~Component() = default;
    };

}