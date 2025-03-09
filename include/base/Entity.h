// Entity.h
// PURPOSE: Define a basic wrapper around entt::

#pragma once
#include <include/thirdparty/entt.hpp>
#include <include/base/Component.h>
#include <include/base/Tag.h>
#include <include/base/Scene.h>
#include <include/base/logging.h>




namespace Quakaster::base {

    /// <summary>
    /// Base entity type written over EnTT. It represents a collection of components guaranteed to exist, 
    /// as well as a series of methods that wrap inter-component functionality.
    /// </summary>
    struct Entity {
    // protected: //Temporarily commented while we make sure the constructors work


        /// <summary>
        /// Initializes a new instance of the Entity class.
        /// </summary>
        /// <param name="scene">Reference to the scene that this entity belongs to.</param>
        Entity(Scene& scene) : scene(scene), ID(scene.registry.create()) {}

        // Destructor for Entity. Cleans up resources.
        virtual ~Entity() {
            scene.registry.destroy(ID);
        };

        // Gets the unique identifier for this entity.
        entt::entity get_ID() { return ID; }


        // Gets a reference to the scene this entity belongs to.
        Scene& get_scene() { return scene; }



        // Gets a component from this entity, if it exists, otherwise, failure
        template <typename T>
        inline T& get_component()
        { return scene.get_component<T>(ID); }


        // Gets a component from this entity. Returns nullptr on failure
        template <typename T>
        inline T* try_get_component()
        { return scene.try_get_component<T>(ID); }

        // Removes the component
        template <typename T>
        inline void remove_component() { scene.remove_component<T>(); }


        // Adds a component to this entity
        template <typename T, typename ...Args>
        inline void add_component(Args&&... args) { scene.add_component<T>(ID, std::forward<Args>(args)...); }

        

        // Returns true if the entity has all of the requested components
        template <typename ...Ts>
        inline bool has() { return scene.has_all_of<Ts...>(ID); }



        // Returns true if the entity has at least one of the following components
        template <typename ...Ts>
        inline bool has_one_of() { return scene.has_one_of<Ts...>(ID); }


    private:

        // A reference to the scene this entity belongs to.
        Scene& scene;

        // The unique identifier for this entity.
        entt::entity ID;


    };

    // Auxillary function to determine if a generic entity is another entity type
    template <typename ent_t>
    inline bool is(const Entity& entity) {
        return entity.has<typename ent_t::tag>();
    }

}

using Entity = Quakaster::base::Entity;