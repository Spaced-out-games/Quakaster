#pragma once
#include <include/GameContext/components/transform.h>
#include <include/GameContext/IO/InputDelegate.h>
#include <include/GameContext/base/eventHandler.h>
#include <include/GameContext/components/camera.h>
#include <include/thirdparty/entt.hpp>
#include <math.h>


struct Controller {

	Controller(EventHandler& event_handler,
		std::function<void(KeyPressEvent)> on_key_press_fn = [](KeyPressEvent) {},
		std::function<void(KeyHoldEvent)> on_key_hold_fn = [](KeyHoldEvent) {},
		std::function<void(KeyReleaseEvent)> on_key_release_fn = [](KeyReleaseEvent) {},
		std::function<void(MouseMoveEvent)> on_mouse_move_fn = [](MouseMoveEvent) {}
	):
		event_handler(event_handler),
		on_key_press(event_handler,on_key_press_fn),
		on_key_hold(event_handler, on_key_hold_fn),
		on_key_release(event_handler, on_key_release_fn),
		on_mouse_move(event_handler, on_mouse_move_fn)
	{}

    protected:
	// These probably need wrapped in functions, but I digress.
	EventListener<KeyPressEvent> on_key_press;
	EventListener<KeyHoldEvent> on_key_hold;
	EventListener<KeyReleaseEvent> on_key_release;
	EventListener<MouseMoveEvent> on_mouse_move;
	EventHandler& event_handler;

	//private:
		


};

class MoveState {
    protected:
        bool mWarp = false;
	    bool mInAir = true;
	    bool mMoving = false;
        glm::vec3 mWishDir = {0.0,0.0,0.0};
        glm::vec3 mVelocity = {0.0,0.0,0.0};
	    float mMaxSpeed = 25.0f;
	    float mAirAccel = 50.0f;
        float mGroundAccel = 50.0f;

    public:
    #pragma region setters
        inline void set_warp(bool new_warp) { mWarp = new_warp; }
        inline void set_in_air(bool new_in_air) { mInAir = new_in_air; }
        inline void set_moving(bool new_moving) { mMoving = new_moving; }
        inline void set_wish_dir(glm::vec3 new_wish_dir) { mWishDir = new_wish_dir; }
        inline void set_velocity(glm::vec3 new_velocity) { mVelocity = new_velocity; }
        inline void set_max_speed(float new_max_speed) { mMaxSpeed = new_max_speed; }
        inline void set_air_accel(float new_air_accel) { mAirAccel = new_air_accel; }
        inline void set_ground_accel(float new_ground_accel) { mGroundAccel = new_ground_accel; }
    #pragma endregion setters
    #pragma region getters
        const inline bool& warp() const { return mWarp; }
        const inline bool& in_air() const { return mInAir; }
        const inline bool& moving() const { return mMoving; }
        const inline glm::vec3& wish_dir() const { return mWishDir; }
        const inline glm::vec3& velocity() const { return mVelocity; }
        const inline float& max_speed() const { return mMaxSpeed; }
        const inline float& air_accel() const { return mAirAccel; }
        const inline float& ground_accel() const { return mGroundAccel; }

    #pragma endregion getters

};


struct test_controller : Controller {
    Entity& entity;
    int i;

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

