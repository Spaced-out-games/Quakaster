#pragma once
#include <iostream>
#include <include/utility_types.h>
#include "entt.hpp"


// For now, define a Scene as a registry. 


class Entity {
private:
    entt::entity ent_ID; // ID of the entity
protected:
    friend Scene;
    static Scene* scene; // Static pointer to the Scene
public:
    template <typename T>
    bool has() {
        return scene->any_of<T>(ent_ID);
    }

    template <typename T, typename... Args>
    void add_component(Args&&... args) {
        scene->emplace<T>(ent_ID, std::forward<Args>(args)...);
    }

    template <typename T>
    void remove_component() {
        scene->remove<T>(ent_ID);
    }

    Entity(entt::entity ID) : ent_ID(ID) {}
    ~Entity(); // Default destructor
};


#include "include/scene.h"


Entity::~Entity()
{
    scene->destroy(ent_ID);
}

Scene* Entity::scene = nullptr;




//Scene* Entity::scene = nullptr;