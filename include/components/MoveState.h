#pragma once
#include <include/server/interpreter/convar.h>
#include <include/components/transform.h>
#include <include/bsp/bsp_plane.h>

//add_convar()

// Once we are ready, just make them replicated convars. These might be player-specific, so it's best that they be there on a per-client basis
Convar sv_movespeed("sv_movespeed", 10.0f);
Convar sv_airaccel("sv_airaccel", 25.0f);
Convar sv_groundaccel("sv_groundaccel", 50.0f);
Convar sv_gravity("sv_gravity", 20.0f);
Convar sv_jumpheight("sv_jumpheight", 5.0f);
Convar sv_airfriction("sv_airfriction", 0.0f);
Convar sv_friction("sv_friction", 8.0f);





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


    glm::vec3 mVelocity = { 0.0,0.0,0.0 };

    float friction(float dt)
    {
        if (in_air()) { return 1.0 - (sv_airfriction.as<float>() * dt); }
        return 1.0 - (sv_friction.as<float>() * dt);
    }

    void jump() {
        set_downward_velocity(sv_jumpheight.as<float>());
    }
};



#include <include/base/Scene.h>





