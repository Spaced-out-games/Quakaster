#pragma once

class MoveState: Component {
protected:
    bool mWarp = false;
    bool mInAir = true;
    bool mMoving = false;
    glm::vec3 mWishDir = { 0.0,0.0,0.0 };
    glm::vec3 mVelocity = { 0.0,0.0,0.0 };
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