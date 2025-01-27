#pragma once
#include <include/thirdparty/glm/glm.hpp>

extern float deltaTime;


// Functions as a state machine. The controller in question is to set some state information remotely (eg, instead of passing values through jump(), it simply sets impulse_vector to (0,1,0), then calls apply_impulse(),
// or inherited classes simply set default states that are used consistently
class IMoveSystem
{
	static void init_default_bindings(std::unordered_map<SDLKeycode, move_fn>& bindings) = 0;
	// store movement state
	bool moving;
	bool in_air;
	bool in_water = 0; // not used
	bool jumping;

	// What impulse to apply when on_impulse is called
	glm::vec3 impulse_vector;

	// What vector to apply when jumping.
	glm::vec3 jump_vector;

	// current mouse position
	glm::ivec2 mouse;

	// change in mouse position
	glm::ivec2 mouse_vel;

	float sv_movespeed = 10.0f;
	float sv_maxspeed = 10.0f; // Just because this is here does not mean it necessarily needs to be used, but its good to have on the table
	float sv_air_acceleration = 50.0f; // Air acceleration factor
	float sv_air_friction_coefficient = 0.0f; // By default, friction doesn't apply in the air
	float sv_ground_accel = 50.0f; // Ground acceleration factor
	float sv_ground_friction_coefficient = 0.5f; // Random ass value, and true meaning is dependent on implementation, perhaps this means slowing the player by 50% per second, or 50% per tick, or while 

	// Reference to the real position of an object
	Transform& transform

	// Which direction the player wishes to go in
	glm::vec3 wishdir;

	// Current velocity of the player
	glm::vec3 velocity;

	const float& get_deltaTime() { return deltaTime; } // so that inherited classes don't need to worry about declaring deltaTime

	public:
		using move_fn = void(IMoveSystem::*)();
		IMoveSystem(glm::vec3& position) : position(position) {}


		void set_velocity(glm::vec3& new_velocity) { velocity = new_velocity; }
		void set_jump_height(float height) { jump_vector.y = height; }
		void set_jump_vector(glm::vec3& jump_direction) { jump_vector = jump_direction; }
		void set_mouse_position(glm::ivec2& mouse_pos) { mouse = mouse_pos; }
		void set_mouse_velocity(glm::ivec2& delta) { mouse_vel = delta; }
		void set_ground_acceleration(float acceleration) { sv_ground_accel = acceleration; }
		void set_air_acceleration(float acceleration) { sv_air_accel = acceleration; }
		void set_wish_direction(glm::vec3& direction) { wish_dir = direction; }



		static void while_in_air() = 0;
		static void jump() = 0;
		static void land() = 0;
		static void moveLeft() = 0;
		static void moveRight() = 0;
		static void moveForward() = 0;
		static void moveBack() = 0;
		static void croutch() = 0;
		static void uncroutch() = 0; // Exact implementations may vary, maybe in some implementations, hitboxes change size, and in others, they do not.
		static void tick() = 0; // Good for updating things like position in respect to velocity. When the game is eventually optimized, this function will be made into a non-static inline function that will be called in parallel per game tick
		static void on_impulse() = 0

};