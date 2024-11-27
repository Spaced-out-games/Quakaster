#pragma once
#include <include/entt.hpp>
#include "scene.h" // Include Scene header



/// <summary>
/// Basic Entity. Inherit from this to construct your own types that add components in their constructor.
/// INHERITED TYPES MUST NOT HAVE ADDITIONAL DATA ENTRIES. THIS ALLOWS STRAIGHTFORWARD AND SAFE DOWNCASTING
/// </summary
class Entity
{
public:
    // Scenes can access Entities
    friend Scene;


    entt::entity ID = entt::null; // Default ID, ideally this should be set via the constructor

    // Pointer to the scene this entity lives in
    Scene* my_scene = nullptr;

    // Utility pointer in case you only need one scene (or are lazy)
    static Scene* default_scene;


    // Constructs, given a scene
    Entity(Scene& scene);

    // Constructs, using the default scene. YOU MUST CALL setCurrentScene() BEFORE USING THIS
    Entity() : Entity(*default_scene) {}

    // Cleans up the entity, destroying components
    ~Entity();


    // Sets a default scene for following Entities that are constructed to use
    static void setCurrentScene(Scene& scene) { default_scene = &scene; }


    // Add a component to the entity
    template <typename T, typename ...Args>
    void add_component(Args&&... args);

    // Remove a component from the entity
    template <typename T>
    void remove_component();
};

Scene* Entity::default_scene = nullptr;

// Constructor
Entity::Entity(Scene& scene) : my_scene(&scene)
{
    ID = my_scene->create();
}

// Destructor
Entity::~Entity()
{
    if (my_scene) {
        my_scene->destroy(ID);  // Destroy the entity from the scene when the entity goes out of scope
    }
}

// Add a component to the entity
template <typename T, typename ...Args>
void Entity::add_component(Args&&... args)
{
    if (my_scene) {
        my_scene->emplace<T>(ID, args...);  // Add the component to the entity
    }
}

// Remove a component from the entity
template <typename T>
void Entity::remove_component()
{
    if (my_scene) {
        my_scene->remove<T>(ID);  // Remove the component from the entity
    }
}
