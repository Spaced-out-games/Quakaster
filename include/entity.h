#pragma once
#include <include/entt.hpp>
#include "scene.h" // Include Scene header

#define ADD_EVENT_LISTENER(dispatcher, event_type, listener) \
    dispatcher.sink<event_type>().connect<&listener>();  // Connect the listener to the dispatcher


template <typename T, typename... Args>
void fire_event(entt::dispatcher& dispatcher, Args&&... args) {
    dispatcher.trigger<T>(T{ std::forward<Args>(args)... }); // Construct the event in place
}

// Overload for passing a pre-constructed event
template <typename T>
void fire_event(entt::dispatcher& dispatcher, const T& event_instance) {
    dispatcher.trigger<T>(event_instance); // Use a copy of the instance
}

template <typename T>
void fire_event(entt::dispatcher& dispatcher, T&& event_instance) {
    dispatcher.trigger<T>(std::forward<T>(event_instance)); // Perfectly forward the instance
}


//#define EVENT_FIRE(dispatcher, event_type, event_inst) 
//    dispatcher.trigger<event_type>(std::move(event_inst));  // Forward event instance directly

/// <summary>
/// Basic Entity. Inherit from this to construct your own types that add components in their constructor.
/// INHERITED TYPES MUST NOT HAVE ADDITIONAL DATA ENTRIES. THIS ALLOWS STRAIGHTFORWARD AND SAFE DOWNCASTING


class Entity
{
public:
    // Scenes can access Entities
    friend Scene;

    bool is_valid() { return ID != entt::null; }


    entt::entity ID = entt::null; // Default ID, ideally this should be set via the constructor

    // Pointer to the scene this entity lives in
    Scene* my_scene = nullptr;

    // Utility pointer in case you only need one scene (or are lazy)
    static Scene* default_scene;


    // Constructs, given a scene
    Entity(Scene* scene = nullptr);

    // If you have entities created before a scene is created, you MUST call this to properly initialize this
    void init();

    // Constructs, using the default scene. YOU MUST CALL setCurrentScene() BEFORE USING THIS
    //Entity() : Entity(*default_scene) {}

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

    // Add an event listener for this entity
    template <typename Event, typename Listener>
    void addEventListener(Listener&& listener);

    // Fire an event from this entity
    template <typename Event, typename... Args>
    void fireEvent(Args&&... args);

    // Get a component from the entity
    template <typename T>
    T& getComponent();
};

Scene* Entity::default_scene = nullptr;

// Constructor
Entity::Entity(Scene* scene) : my_scene(scene)
{
    init();
}

void Entity::init()
{
    if (my_scene && ID == entt::null) ID = my_scene->create();
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

// Add an event listener for this entity
template <typename Event, typename Listener>
void Entity::addEventListener(Listener&& listener) {
    if (my_scene) {
        auto sink = my_scene->dispatcher.sink<Event>();
        // Use std::forward to forward the listener correctly
        sink.connect(std::forward<Listener>(listener));  // Connect the listener
    }
}


// Fire an event from this entity
template <typename Event, typename... Args>
void Entity::fireEvent(Args&&... args) {
    if (my_scene) {
        my_scene->dispatcher.trigger<Event>(std::forward<Args>(args)...);
    }
}

// Get a component from the entity
template <typename T>
T& Entity::getComponent() {
    if (my_scene) {
        return my_scene->get<T>(ID);  // Retrieve the component from the scene
    }
    throw std::runtime_error("Entity does not have the requested component.");
}