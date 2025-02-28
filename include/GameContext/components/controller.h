#pragma once
#include <include/GameContext/components/camera.h>
#include <include/GameContext/IO/controller.h>
#include <include/thirdparty/entt.hpp>
#include <math.h>

extern float deltaTime;

struct ent_controller : public EventListener {
    std::unordered_map<SDL_Keycode, MoveSystem::move_fn> bindings;
    IMoveSystem& movement_system;

    ent_controller(eventHandler& dispatcher, Transform& transform)
        : EventListener(dispatcher, ALL_EVENTS), target_transform(transform)
    {
        movement_system.init_default_bindings(bindings);
        // These are functions that 
        on_keyPress = [this](KeyPressEvent& evt) {
            bindings[evt.code](movement_system);

        };

        on_keyRelease = [this](KeyReleaseEvent& evt) {

        };

        on_keyHold = [this](KeyHoldEvent& evt) {

        };

        on_mouseMove = [this](MouseMoveEvent& evt) {
            movement_system.set_mouse_velocity(0.01f * -evt.xrel, 0.01f * -evt.yrel);
            // Calculate pitch and yaw deltas from mouse input
            float pitchDelta = 0.01f * -evt.yrel;
            float yawDelta = 0.01f * -evt.xrel;

            // Update the target transform's rotation using quaternions
            glm::quat pitchQuat = glm::angleAxis(pitchDelta, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X
            glm::quat yawQuat = glm::angleAxis(yawDelta, glm::vec3(0.0f, 1.0f, 0.0f));     // Rotate around Y

            // Combine pitch and yaw rotations with the current rotation
            target_transform.rotation = glm::normalize(yawQuat * target_transform.rotation * pitchQuat);
        };
    }

};
