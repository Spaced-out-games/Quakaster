#pragma once
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/utils/utility_types.h>
#include <functional>
#include <include/GameContext/utils/Inspector.h>
#include <chrono>



class ent {
public:
    using InitFunction = std::function<void(entt::handle&)>;
    using ThinkFunction = std::function<void(entt::handle&)>;

    #ifdef _DEBUG
    Inspector inspector{ this };
    std::string inspect();
    #endif

    // Default constructor
    ent() : handle(), tick_interval(0), last_tick_time(std::chrono::steady_clock::now()) {}

    // Constructor that constructs the handle in place and accepts init and think functions
    ent(entt::registry& registry,
        InitFunction initFunc = default_init,
        ThinkFunction thinkFunc = default_think,
        double tick_interval_seconds = 0.0)
        : handle{ registry, registry.create() },
        initFunc(initFunc),
        thinkFunc(thinkFunc),
        tick_interval(tick_interval_seconds),
        last_tick_time(std::chrono::steady_clock::now()) {
        init(); // Call the initialization function
    }

    // Move constructor
    ent(ent&& other) noexcept : handle(std::move(other.handle)),
        initFunc(std::move(other.initFunc)),
        thinkFunc(std::move(other.thinkFunc)),
        tick_interval(other.tick_interval),
        last_tick_time(std::move(other.last_tick_time)) {}

    // Set the think function
    void set_think(ThinkFunction think) {
        thinkFunc = think;
    }

    // Add a component to the entity
    template<typename T, typename... Args>
    void add(Args&&... args) {
        handle.emplace<T>(std::forward<Args>(args)...);

        #ifdef _DEBUG
        // Register the string representation for this component
        inspector.register_hook(typeid(T).name(), [this](void*) {
            return "Component<" + std::string(typeid(T).name()) + ">";
        });
        #endif
    }

    // Get a component from the entity
    template<typename T>
    T& get() {
        return handle.get<T>();
    }

    // Remove a component from the entity
    template<typename T>
    void remove() {
        handle.remove<T>();
    }

    // Check if the entity has all specified components
    template<typename... Components>
    bool has_all_of() const {
        return handle.all_of<Components...>();
    }

    // Get the handle for the entity
    entt::handle get_handle() const {
        return handle;
    }

    // Call the think function, but only if the interval has passed
    void tick() {
        auto now = std::chrono::steady_clock::now();
        double elapsed_seconds = std::chrono::duration<double>(now - last_tick_time).count();

        if (tick_interval == 0.0 || elapsed_seconds >= tick_interval) {
            thinkFunc(handle);
            last_tick_time = now;
        }
    }

private:
    entt::handle handle;              // The handle to the entity in the registry
    InitFunction initFunc;            // Function to initialize components
    ThinkFunction thinkFunc;          // Function to update the entity
    double tick_interval;             // Time interval in seconds for calling `think`
    std::chrono::steady_clock::time_point last_tick_time; // Last tick timestamp

    // Default initialization function (no-op)
    static void default_init(entt::handle&) {}

    // Default think function (no-op)
    static void default_think(entt::handle&) {}

    // Call the initialization function
    void init() {
        initFunc(handle);
    }
};

#ifdef _DEBUG
// Access the string representation of the entity's introspection. Only in debug!!
std::string ent::inspect() {
    std::string result;
    result += "Entity " + std::to_string(entt::to_integral(handle.entity())) + ":\n";
    result += inspector["struct EventListener"] + "\n"; // Replace with actual component names
    return result;
}
#endif


