#pragma once
#include <include/GameContext/components/camera.h>
#include <include/GameContext/IO/controller.h>
#include <include/thirdparty/entt.hpp>
#include <math.h>


extern float deltaTime;

struct ent_controller : public EventListener {
    Transform& target_transform;
    glm::vec3 wish_dir;
    glm::vec3 velocity = { 0.0f, 0.0f, 0.0f }; // test
    bool warp = false;
    bool moving = false;
    bool in_air = true; // Change based on collision detection
    float speed = 20.0f;
    float air_accel = 50.0f; // Air acceleration factor
    float ground_accel = 50.0f; // Ground acceleration factor

    ent_controller(entt::dispatcher& dispatcher, Transform& transform)
        : EventListener(dispatcher, ALL_EVENTS), target_transform(transform)
    {
        on_keyPress = [this](KeyPressEvent& evt) {
            switch (evt.code)
            {
            case SDLK_SPACE:
                in_air ^= 1;
                break;
            default:
                break;
            }
            handleMovement<KeyPressEvent>(evt);
            if (moving)
            {
                //applyMovement();

            }
        };

        on_keyRelease = [this](KeyReleaseEvent& evt) {
            switch (evt.code)
            {
            case SDLK_w:
                moving = false;
                break;
            case SDLK_a:
                moving = false;
                break;
            case SDLK_s:
                moving = false;
                break;
            case SDLK_d:
                moving = false;
                break;
            
            default:
                break;

            }
            //moving = false; // Stop moving when the key is released
        };

        on_keyHold = [this](KeyHoldEvent& evt) {
            handleMovement<KeyHoldEvent>(evt);
            if (moving)
            {
                applyMovement();

            }
        };

        on_mouseMove = [this](MouseMoveEvent& evt) {
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
    template <typename T>
    void handleMovement(T& evt) {
        if(!moving) wish_dir = { 0.0f, 0.0f, 0.0f };
        Transform& transform = target_transform;

        switch (evt.code) {
        case SDLK_a:
            moving = true;
            wish_dir -= transform.get_right_vector() * glm::vec3{1.0,0.0,1.0};
            break;
        case SDLK_d:
            moving = true;
            wish_dir += transform.get_right_vector() * glm::vec3{ 1.0,0.0,1.0 };
            break;
        case SDLK_w:
            moving = true;
            wish_dir += transform.get_forward_vector() * glm::vec3{ 1.0,0.0,1.0 };
            break;
        case SDLK_s:
            moving = true;
            wish_dir -= transform.get_forward_vector() * glm::vec3{ 1.0,0.0,1.0 };
            break;
        case SDLK_ESCAPE:
            break;
        default:
            //moving = false;
            break;
        }

        
    }

    void applyMovement() {
        if (moving) {
            if (!in_air) {
                // Apply ground acceleration
                applyGroundAcceleration();
            }
            else {
                // Apply air acceleration
                applyAirAcceleration();
            }

            // Move the transform based on velocity
            //target_transform.move(velocity * deltaTime);
        }
    }

    void applyGroundAcceleration() {
        // Calculate the current speed in the wish direction
        float currentSpeed = glm::dot(velocity, glm::normalize(wish_dir));
        float addSpeed = speed - currentSpeed;

        if (addSpeed <= 0.0f) return; // Already at or above desired speed

        // Acceleration to be applied
        float accelSpeed = ground_accel * deltaTime;

        // Cap acceleration to the required additional speed
        if (accelSpeed > addSpeed) {
            accelSpeed = addSpeed;
        }

        // Apply acceleration
        velocity += accelSpeed * glm::normalize(wish_dir);
    }


    void applyAirAcceleration() {
        // Calculate the current speed in the wish direction
        float currentSpeed = glm::dot(velocity, glm::normalize(wish_dir));
        float addSpeed = speed - currentSpeed;

        if (addSpeed <= 0.0f) return; // Already at or above desired speed

        // Acceleration to be applied
        float accelSpeed = air_accel * deltaTime;

        // Cap acceleration to the required additional speed
        if (accelSpeed > addSpeed) {
            accelSpeed = addSpeed;
        }

        // Apply acceleration
        velocity += accelSpeed * glm::normalize(wish_dir);
    }

};
