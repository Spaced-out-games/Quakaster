
class IMovementModule
{
	IMovementModule(glm::vec3& position) : position(position) {}
	// store movement state
	bool moving;
	bool in_air;
	bool in_water = 0; // not used
	bool jumping;
	float sv_movespeed;
	float sv_air_acceleration = 50.0f; // Air acceleration factor
	float sv_ground_accel = 50.0f; // Ground acceleration factor


	glm::vec3& position;
	glm::vec3 wishdir;
	glm::vec3 velocity;

	virtual void on_jump();
	virtual void on_land();
	virtual void on_begin_move();
	virtual void on_tick();
	
};