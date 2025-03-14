#pragma once
#include <include/server/interpreter/convar.h>
#include <include/components/transform.h>


//add_convar()

Convar sv_movespeed("sv_movespeed", 10.0f);
Convar sv_airaccel("sv_airaccel", 25.0f);
Convar sv_groundaccel("sv_groundaccel", 50.0f);
Convar sv_gravity("sv_gravity", 20.0f);
Convar sv_jumpheight("sv_jumpheight", 5.0f);





class MoveState: QKComponent {
protected:
    //Convar teleport("tp", [&](void(std::stringstream&) {});

    bool mWarp = false;
    bool mInAir = true;
    bool mMoving = false;
    glm::vec3 mWishDir = { 0.0,0.0,0.0 };
    float* mMaxSpeed = &sv_movespeed.m_Value.m_Float;
    float* mAirAccel = &sv_airaccel.m_Value.m_Float;
    float* mGroundAccel = &sv_groundaccel.m_Value.m_Float;
    float* mGravity = &sv_gravity.m_Value.m_Float;
    float* mJumpHeight = &sv_jumpheight.m_Value.m_Float;

public:
    #pragma region setters
    inline void set_warp(bool new_warp) { mWarp = new_warp; }
    inline void set_in_air(bool new_in_air) { mInAir = new_in_air; }
    inline void set_moving(bool new_moving) { mMoving = new_moving; }
    inline void set_wish_dir(glm::vec3 new_wish_dir) { mWishDir = new_wish_dir; }
    inline void set_velocity(glm::vec3 new_velocity) { mVelocity = new_velocity; }
    inline void set_max_speed(float new_max_speed) { *mMaxSpeed = new_max_speed; }
    inline void set_air_accel(float new_air_accel) { *mAirAccel = new_air_accel; }
    inline void set_ground_accel(float new_ground_accel) { *mGroundAccel = new_ground_accel; }
    #pragma endregion setters
    #pragma region getters
    inline bool warp() const { return mWarp; }
    inline bool in_air() const { return mInAir; }
    inline bool moving() const { return mMoving; }
    inline glm::vec3 wish_dir() const { return mWishDir; }
    inline glm::vec3 velocity() const { return mVelocity; }
    inline float max_speed() const { return *mMaxSpeed; }
    inline float air_accel() const { return *mAirAccel; }
    inline float ground_accel() const { return *mGroundAccel; }
    inline void scale_velocity(float scalar) { mVelocity *= scalar; }
    inline void scale_velocity_horizontal(float scalar) { mVelocity.x *= scalar; mVelocity.z *= scalar; }
    float gravity() { return *mGravity; }
    inline float get_downward_velocity() { return mVelocity.y; }
    inline void set_downward_velocity(float downward_velocity) { mVelocity.y = downward_velocity; }
    #pragma endregion getters

    float airFriction = 1.0;
    float groundFriction = 0.998;
    glm::vec3 mVelocity = { 0.0,0.0,0.0 };

    float friction()
    {
        //return 1.0;
        if (in_air()) { return airFriction; }
        return groundFriction;
    }

    void jump() {
        set_downward_velocity(sv_jumpheight.as<float>());
    }
};



#include <include/base/Scene.h>


// Applies physics to every entity that needs it
struct MoveState_system: ISystem {
    void init(QKScene&) override {}
    void destroy(QKScene&) override {}


    void tick(QKScene& scene) override {
        auto view = scene.view<MoveState, Transform>();
        float dt = (float)Application::get_deltaTime();
        for (auto entity : view) {
            MoveState& ms = scene.get<MoveState>(entity);
            Transform& tf = scene.get<Transform>(entity);

            // Update downward velocity based on gravity
            ms.set_downward_velocity(ms.get_downward_velocity() - (ms.gravity() * (float)Application::get_deltaTime()));

            // Update position based on velocity
            tf.position += ms.velocity() * (float)Application::get_deltaTime();

            // Check for collision with the ground
            if (tf.position.y < 2.f) {
                ms.set_in_air(false);
                ms.set_downward_velocity(0); // Reset downward velocity
                tf.position.y = 2.f; // Set position to ground level
            }
            else {
                ms.set_in_air(true);
            }
            ms.scale_velocity_horizontal(ms.friction());

        }
    }


};