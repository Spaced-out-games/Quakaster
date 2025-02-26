#pragma once
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/base/Component.h>
#include <include/GameContext/base/Tag.h>
#include <include/GameContext/base/Scene.h>


// Forward declaration
struct Scene;



namespace Quakaster::base {

    /// <summary>
    /// Base entity type written over EnTT. It represents a collection of components guaranteed to exist, 
    /// as well as a series of methods that wrap inter-component functionality.
    /// </summary>
    struct Entity {
    // protected: //Temporarily commented while we make sure the constructors work
        /// <summary>
        /// Grants access to Entity's members to the Scene struct.
        /// </summary>
        friend struct Scene;

        /// <summary>
        /// Initializes a new instance of the Entity class.
        /// </summary>
        /// <param name="id">The entity identifier.</param>
        /// <param name="scene">Reference to the scene that this entity belongs to.</param>
        /// <param name="tag_t"> an Tag-derived type denoting the entity type of this entity (eg, tag_player for a Player entity) </param>
        template <typename tag_t = Tag>
        Entity(entt::entity id, Scene& scene) : ID(id), scene(scene) {
            scene.registry.emplace<tag_t>(id);
        }

    public:
        /// <summary>
        /// Destructor for Entity. Cleans up resources.
        /// </summary>
        virtual ~Entity() = default;
        /// <summary>
    /// Gets the unique identifier for this entity.
    /// </summary>
        entt::entity get_ID() { return ID; }

        /// <summary>
        /// Gets a reference to the scene this entity belongs to.
        /// </summary>
        Scene& get_scene() { return scene; }


        /// <summary>
        /// Gets a component from this entity
        /// </summary>
        /// <typeparam name="T">The type of component you're looking for</typeparam>
        /// <returns>A reference to the component</returns>
        template <typename T>
        inline T& get_component()
        { return scene.get_component<T>(ID); }

        /// <summary>
        /// Gets a componne from this entity. Returns nullptr on failure
        /// </summary>
        /// <typeparam name="T">The type of component you're looking for</typeparam>
        /// <returns>A pointer to the component</returns>
        template <typename T>
        inline T* try_get_component()
        { return scene.try_get_component<T>(ID); }



        /// <summary>
        /// Adds a component to this entity
        /// </summary>
        /// <typeparam name="T">The type of component to append to this entity</typeparam>
        /// <typeparam name="...Args">parameter type list for the constructor of the component</typeparam>
        /// <param name="...args">parameter value list for the constructor of the component</param>
        template <typename T, typename ...Args>
        inline void add_component(Args&&... args) { scene.add_component<T>(ID, std::forward<Args>(args)...); }

    private:
        /// <summary>
        /// A reference to the scene this entity belongs to.
        /// </summary>
        Scene& scene;

        /// <summary>
        /// The unique identifier for this entity.
        /// </summary>
        entt::entity ID;


    };

}