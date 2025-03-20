#pragma once
#include "Controller.h"
#include "MoveState.h"

extern Convar sv_autohop;
extern Convar sv_airfriction;


struct Freecam : Controller, QKComponent {



    // Applies physics to every entity that needs it
    struct system : ISystem {
        void init(QKScene&) override {}
        void destroy(QKScene&) override {}

        //static inline BSP_plane ground_plane = { glm::normalize(glm::vec3(0, 1, 0)), 0.0f }; // Defines the ground plane at Y = 0

        void tick(QKScene& scene) override {

            auto view = scene.view<MoveState, Transform, Freecam>();
            float dt = static_cast<float>(Application::get_deltaTime());

            for (auto entity : view) {
                MoveState& ms = scene.get<MoveState>(entity);
                Transform& tf = scene.get<Transform>(entity);



                // Update position based on velocity
                tf.position += ms.velocity() * dt;

                // Check collision with BSP plane
                //float distance_from_plane = glm::dot(ground_plane.normal, tf.position) - ground_plane.distance;



                // Apply horizontal friction
                ms.scale_velocity(ms.friction(dt));
                //ms.scale_velocity_horizontal(ms.friction(dt));
            }

        }
    };







    inline MoveState& get_move_state() { return entity.get<MoveState>(); }

    Freecam(QKEntity& entity) : Controller(), entity(entity) {
        on_key_press.on_event = [&](KeyPressEvent evt) {
            auto& ms = get_move_state();


            switch (evt.code) {
            case SDLK_SPACE:
                if (!ms.in_air()) ms.jump();
                break;
            default:
                break;
            }

            handle_movement(evt.code, ms);

        };

        sv_airfriction.m_Value.m_Float = 2.0f;

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

            switch (evt.code) {
            case SDLK_SPACE:
                if (!ms.in_air() && sv_autohop.m_Value.m_Int != 0) {
                    ms.jump();
                }
                break;
            default:
                break;
            }

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

            entity.get<Camera>().owner_transform.rotation = glm::normalize(yawQuat * entity.get<Camera>().owner_transform.rotation * pitchQuat);
        };

        on_mouse_click.on_event = [&](MouseClickEvent evt) {
            auto& ms = get_move_state();
            if (evt.button == SDL_BUTTON_LEFT)
            {
                ms.mVelocity -= entity.get<Camera>().owner_transform.get_forward_vector() * 10.0f;
            }

        };

    }

    // Updates velocity in accordance to move state
    inline void handle_movement(SDL_Keycode code, MoveState& ms);


    inline void apply_movement(MoveState& ms, QKEntity& entity);

    inline void apply_ground_accel(MoveState& ms);

    static void apply_air_accel(MoveState& ms);

private:
    QKEntity& entity;
};



void Freecam::handle_movement(SDL_Keycode code, MoveState& ms) {
    if (!ms.moving()) { ms.set_wish_dir(glm::vec3{ 0.0f, 0.0f, 0.0f }); }
    Transform& transform = entity.get<Camera>().owner_transform; // easier to test immediately

    switch (code) {
    case SDLK_a:
        ms.set_moving(true);
        ms.set_wish_dir(-transform.get_right_vector());// only move in the horizontal axis
        break;
    case SDLK_d:
        ms.set_moving(true);
        ms.set_wish_dir(transform.get_right_vector()); // only move in the horizontal axis
        break;
    case SDLK_w:
        ms.set_moving(true);
        ms.set_wish_dir(transform.get_forward_vector()); // only move in the horizontal axis
        break;
    case SDLK_s:
        ms.set_moving(true);
        ms.set_wish_dir(-transform.get_forward_vector()); // only move in the horizontal axis
        break;
    case SDLK_ESCAPE:
        break; // No action for escape
    default:
        // moving = false
        break; // Add default case
    }
}

inline void Freecam::apply_movement(MoveState& ms, QKEntity& entity) {
    Transform& transform = entity.get<Camera>().owner_transform; // easier to test immediately
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
}

inline void Freecam::apply_ground_accel(MoveState& ms) {
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

void Freecam::apply_air_accel(MoveState& ms) {
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