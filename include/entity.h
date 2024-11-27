#pragma once
#include <include/entt.hpp>
#include "scene.h" // Include Scene header

class Entity
{
public:
    friend Scene;
    entt::entity ID = (entt::entity)99999; // Default ID, ideally this should be set via the constructor
    Scene* my_scene = nullptr;
    static Scene* default_scene;

    Entity(Scene& scene);
    Entity() : Entity(*default_scene) {}
    ~Entity();

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
Entity::Entity(Scene& scene): my_scene(&scene)
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
