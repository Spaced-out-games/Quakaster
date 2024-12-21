#pragma once
#include <include/entt.hpp>
#include <include/think.h>




class Entity;
class Application;


/// <summary>
/// Scene containing all the objects in the game world. Might be a good idea to allow the Scene to have access to the GameContext (NOT IMPLEMENTED)
/// for access to things like rendering, GUI shit, window state information... 
/// </summary>
/// 

class Scene {
private:

public:
    entt::registry registry;
    entt::dispatcher dispatcher;
    Application* owner = nullptr;

    // Functions to be called every tick iteration
    std::vector<thinkScript> scripts;

    // Access the registry
    entt::registry& get_registry() { return registry; }
    const entt::registry& get_registry() const { return registry; }

    // Attempts to call all think scripts, but only succeeds if scheduled
    inline void tick() {
        for (thinkScript& script : scripts) {
            if (script.shouldTick()) {
                //script(*this);
                script.advanceTick();
            }
        }
    }

    // Adds a think script to the game world
    inline void addThink(func_ptr_t<void, entt::registry&> func, int tps = 60) {
        scripts.emplace_back(func, tps);
    }

    // Fire an event by constructing it in place
    template <typename T, typename... Args>
    inline void fire_event(Args&&... args) {
        dispatcher.trigger<T>(T{ std::forward<Args>(args)... });
    }

    // Overload for passing a pre-constructed event (by copy or perfect forwarding)
    template <typename T>
    inline void fire_event(const T& event_instance) {
        dispatcher.trigger<T>(event_instance);
    }

    template <typename T>
    inline void fire_event(T&& event_instance) {
        dispatcher.trigger<T>(std::forward<T>(event_instance));
    }

    // Forward entt::registry methods
    template <typename... Args>
    auto create(Args&&... args) {
        return registry.create(std::forward<Args>(args)...);
    }

    void destroy(entt::entity entity) {
        registry.destroy(entity);
    }

    template <typename Component, typename... Args>
    auto emplace(entt::entity entity, Args&&... args) {
        return registry.emplace<Component>(entity, std::forward<Args>(args)...);
    }

    template <typename Component>
    void remove(entt::entity entity) {
        registry.remove<Component>(entity);
    }

    template <typename Component>
    bool has(entt::entity entity) const {
        return registry.any_of<Component>(entity);
    }

    template <typename Component>
    auto& get(entt::entity entity) {
        return registry.get<Component>(entity);
    }

    template <typename Component>
    auto* try_get(entt::entity entity) {
        return registry.try_get<Component>(entity);
    }

    template <typename... Components>
    auto view() {
        return registry.view<Components...>();
    }
};

