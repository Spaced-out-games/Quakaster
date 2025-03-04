#pragma once
#include "Controller.h"
#include "MoveState.h"




struct test_controller : Controller, Component {
    Entity& entity;

    inline MoveState& get_move_state() { return entity.get_component<MoveState>(); }

    test_controller(EventHandler& event_handler, Entity& entity) : Controller(event_handler), entity(entity) {
        on_key_press.on_event = [&](KeyPressEvent evt) {
            auto& ms = get_move_state();


            switch (evt.code) {
                case SDLK_SPACE:
                    ms.set_in_air(!ms.in_air());
                    break;
                default:
                    break;
            }

            handle_movement(evt.code, ms);
            if (ms.moving()) {
                //entity.get_component<Camera>().owner_transform.position += ms.wish_dir * Application::get_deltaTime() * ms.max_speed;
                // apply_movement(ms, entity);
            }
        };

        
        on_key_release.on_event = [&](KeyReleaseEvent evt) {
            auto& ms = get_move_state();
            switch (evt.code)
            {
            case SDLK_w:
                ms.set_moving(false);

                break;
            case SDLK_a:
                ms.set_moving(false);
                break;
            case SDLK_s:
                ms.set_moving(false);
                break;
            case SDLK_d:
                ms.set_moving(false);
                break;

            default:
                break;

            }
            //moving = false; // Stop moving when the key is released
        };

        on_key_hold.on_event = [&](KeyHoldEvent evt) {
            auto& ms = get_move_state();
            handle_movement(evt.code, ms);
            if (ms.moving())
            {
                apply_movement(ms, entity);

            }

            std::cout << ms.velocity() << '\n';
        };


        on_mouse_move.on_event = [&](MouseMoveEvent evt) {
            // Calculate pitch and yaw deltas from mouse input
            float pitchDelta = 0.01f * -evt.relative_position.y;
            float yawDelta = 0.01f * -evt.relative_position.x;

            // Update the target transform's rotation using quaternions
            glm::quat pitchQuat = glm::angleAxis(pitchDelta, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X
            glm::quat yawQuat = glm::angleAxis(yawDelta, glm::vec3(0.0f, 1.0f, 0.0f));     // Rotate around Y

            entity.get_component<Camera>().owner_transform.rotation = glm::normalize(yawQuat * entity.get_component<Camera>().owner_transform.rotation * pitchQuat);
        };

    }

    // Updates velocity in accordance to move state
    inline void handle_movement(SDL_Keycode code, MoveState& ms) {
        if (!ms.moving()) { ms.set_wish_dir(glm::vec3{ 0.0f, 0.0f, 0.0f }); }
        Transform& transform = entity.get_component<Camera>().owner_transform; // easier to test immediately

        switch (code) {
        case SDLK_a:
            ms.set_moving(true);
            ms.set_wish_dir(ms.wish_dir() - transform.get_right_vector() * glm::vec3{ 1.0, 0.0, 1.0 });// only move in the horizontal axis
            break;
        case SDLK_d:
            ms.set_moving(true);
            ms.set_wish_dir(ms.wish_dir() + transform.get_right_vector() * glm::vec3{ 1.0, 0.0, 1.0 }); // only move in the horizontal axis
            break;
        case SDLK_w:
            ms.set_moving(true);
            ms.set_wish_dir(ms.wish_dir() + transform.get_forward_vector() * glm::vec3{ 1.0, 0.0, 1.0 }); // only move in the horizontal axis
            break;
        case SDLK_s:
            ms.set_moving(true);
            ms.set_wish_dir(ms.wish_dir() - transform.get_forward_vector() * glm::vec3{ 1.0, 0.0, 1.0 }); // only move in the horizontal axis
            break;
        case SDLK_ESCAPE:
            break; // No action for escape
        default:
            // moving = false
            break; // Add default case
        }
    }

    
    inline void apply_movement(MoveState& ms, Entity& entity) {
        Transform& transform = entity.get_component<Camera>().owner_transform; // easier to test immediately
        if (ms.moving()) {
            if (ms.in_air()) {
                // apply air acceleration
                apply_air_accel(ms);
            }
            else {
                // apply ground acceleration
                apply_ground_accel(ms);
            }
        }
        // Move the transform based on velocity
        // target_transform.move(ms.velocity * deltaTime);
        //std::cout << 
        //transform.position += ms.wish_dir * Application::get_deltaTime();
    }

    inline void apply_ground_accel(MoveState& ms) {
        // Ensure wish_dir is not a zero vector
        if (glm::length(ms.wish_dir()) > 0.0f) {
            ms.set_wish_dir(glm::normalize(ms.wish_dir()));
        }
        else {
            return; // No movement direction, exit early
        }

        // Calculate the current speed in the wish direction
        float currentSpeed = glm::dot(ms.velocity(), ms.wish_dir());
        float addSpeed = ms.max_speed() - currentSpeed;

        if (addSpeed <= 0.0f) return; // Already at or above desired speed

        // Acceleration to be applied
        float accelSpeed = ms.ground_accel() * Application::get_deltaTime();


        // Cap acceleration to the required additional speed
        if (accelSpeed > addSpeed) {
            accelSpeed = addSpeed;
        }

        // Apply acceleration
        ms.set_velocity(ms.velocity() + accelSpeed * ms.wish_dir());
        //ms.velocity += accelSpeed * ms.wish_dir;
    }

    static void apply_air_accel(MoveState& ms) {
        // Ensure wish_dir is not a zero vector
        if (glm::length(ms.wish_dir()) > 0.0f) {
            ms.set_wish_dir(glm::normalize(ms.wish_dir()));
        }
        else {
            return; // No movement direction, exit early
        }

        // Calculate the current speed in the wish direction
        float currentSpeed = glm::dot(ms.velocity(), ms.wish_dir());
        float addSpeed = ms.max_speed() - currentSpeed;

        if (addSpeed <= 0.0f) return; // Already at or above desired speed

        // Acceleration to be applied
        float accelSpeed = ms.air_accel() * Application::get_deltaTime();

        // Cap acceleration to the required additional speed
        if (accelSpeed > addSpeed) {
            accelSpeed = addSpeed;
        }

        // Apply acceleration
        ms.set_velocity(ms.velocity() + accelSpeed * ms.wish_dir());

    }
    
};

