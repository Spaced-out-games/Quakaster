#pragma once
#include <include/thirdparty/glm/vec3.hpp>

// Generic Vec3 component
struct Vec3 {
    glm::vec3 value;

    // Constructors
    Vec3() : value(0.0f) {}
    Vec3(float v) : value(v) {}
    Vec3(float x, float y, float z) : value(x, y, z) {}
    Vec3(glm::vec3 v) : value(v) {}

    // Implicit conversion to glm::vec3
    inline operator glm::vec3() const { return value; }

    // Compute dot product
    inline float dot(const glm::vec3& other) const {
        return glm::dot(value, other);
    }

    // Compute cross product
    inline Vec3 cross(const glm::vec3& other) const {
        return Vec3(glm::cross(value, other));
    }

    // Get magnitude (length)
    inline float length() const {
        return glm::length(value);
    }

    // Normalize vector
    inline Vec3 normalized() const {
        return Vec3(glm::normalize(value));
    }

    // Scale vector by scalar value
    inline Vec3 scale(float s) const {
        return Vec3(value * s);
    }

    // Operator overloads
    inline Vec3 operator+(const Vec3& other) const {
        return Vec3(value + other.value);
    }

    inline Vec3& operator+=(const Vec3& other) {
        value += other.value;
        return *this;
    }
};

// Scalar component for storing a single 3D vector value
struct Scalar : public QKComponent {
    Vec3 value;

    Scalar() : value(0.0f) {}
    Scalar(float x, float y, float z) : value(x, y, z) {}
    Scalar(glm::vec3 v) : value(v) {}

    inline operator glm::vec3() const { return value; }

    // Retrieve as glm::vec3
    inline glm::vec3 getValue() const { return value; }

    // Set new value
    inline void setValue(const glm::vec3& v) { value = v; }

    // Get magnitude (length)
    inline float length() const {
        return value.length();
    }
};

// Translation component for positional transformations
struct Translation : QKComponent {
    Vec3 position;

    Translation() : position(0.0f) {}
    Translation(float x, float y, float z) : position(x, y, z) {}
    Translation(glm::vec3 v) : position(v) {}

    inline operator glm::vec3() const { return position; }

    // Compute translation offset
    inline void translate(const glm::vec3& offset) {
        position += offset;
    }
};

// Rotation component using quaternion representation
struct Rotator : QKComponent {
    glm::quat rotation;

    Rotator() : rotation(1, 0, 0, 0) {} // Identity quaternion
    Rotator(glm::quat q) : rotation(q) {}

    inline operator glm::quat() const { return rotation; }

    // Rotate vector by quaternion
    inline glm::vec3 rotateVector(const glm::vec3& v) const {
        return rotation * v;
    }

    // Convert to Euler angles
    inline glm::vec3 toEuler() const {
        return glm::eulerAngles(rotation);
    }

    // Apply rotation by another quaternion
    inline void rotate(const glm::quat& q) {
        rotation = glm::normalize(q * rotation);
    }
};

// Control Point component for utilities like visualization
struct ControlPoint : public QKComponent {
    Vec3 position;

    ControlPoint() : position(0.0f) {}
    ControlPoint(float x, float y, float z) : position(x, y, z) {}
    ControlPoint(glm::vec3 v) : position(v) {}

    inline operator glm::vec3() const { return position; }

    // Translate control point
    inline void move(const glm::vec3& delta) {
        position += delta;
    }
};


struct Matrix {
    glm::mat4 mat;

    Matrix() : mat(1.0f) {}
    explicit Matrix(const glm::mat4 m) : mat(m) {}

    // Scale method with chaining
    inline Matrix& scale(const glm::vec3 scalar) {
        mat = glm::scale(mat, scalar);
        return *this; // Return reference to the current object
    }

    // Scale method with chaining
    inline Matrix& scale(float x, float y, float z) {
        mat = glm::scale(mat, glm::vec3{ x,y,z });
        return *this; // Return reference to the current object
    }

    // Translate method with chaining
    inline Matrix& translate(const glm::vec3 offset) {
        mat = glm::translate(mat, offset);
        return *this; // Return reference to the current object
    }
    // Translate method with chaining
    inline Matrix& translate(float x, float y, float z) {
        mat = glm::translate(mat, glm::vec3{ x,y,z });
        return *this; // Return reference to the current object
    }

    // Rotate method with chaining
    inline Matrix& rotate(float angle, const glm::vec3 axis) {
        mat = glm::rotate(mat, angle, axis);
        return *this; // Return reference to the current object
    }

    // Implicit conversion operator to glm::mat4
    inline operator glm::mat4() const {
        return mat;
    }
};