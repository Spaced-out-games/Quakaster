#pragma once
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/base/Component.h>
#include <include/GameContext/base/Tag.h>

//using namespace Quakaster::base;

// Forward declaration
struct Scene;


namespace Quakaster::base {

    /// <summary>
    /// Base entity type written over EnTT. It represents a collection of components guaranteed to exist, 
    /// as well as a series of methods that wrap inter-component functionality.
    /// </summary>
    struct Entity {
    protected:
        /// <summary>
        /// Grants access to Entity's members to the Scene struct.
        /// </summary>
        friend struct Scene;

        /// <summary>
        /// Initializes a new instance of the Entity class.
        /// </summary>
        /// <param name="id">The entity identifier.</param>
        /// <param name="scene">Reference to the scene that this entity belongs to.</param>
        /// <param name="tag_t"> an ITag-derived type denoting the entity type of this entity (eg, tag_player for a Player entity) </param>
        template <typename tag_t = ITag>
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