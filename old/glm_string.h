#pragma once
#include "glm_master.h"
#include <string>

//using std::string;

// Might want to add `bool pure' to the to_string() methods so that it will return the string without 'typename()`, eg, `vec2(2.0,4.0)` => `2.0,4.0`

std::string to_string(const glm::vec2& vec) {
    std::ostringstream oss;
    oss << "Vector2 (" << vec.x << ", " << vec.y << ")";
    return oss.str();
}

// Function to convert glm::vec3 to std::string
std::string to_string(const glm::vec3& vec) {
    std::ostringstream oss;
    oss << "Vector3 (" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return oss.str();
}

// Function to convert glm::quat to std::string
std::string to_string(const glm::quat& quat) {
    std::ostringstream oss;
    oss << "Quaternion (" << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z << ")";
    return oss.str();
}

// Function to convert glm::mat4 to std::string
std::string to_string(const glm::mat4& mat) {
    std::ostringstream oss;

    // Push the rows of the matrix into the string stream
    oss << "         [ " << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", " << mat[0][3] << " ]\n"
        << "    mat4 [ " << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", " << mat[1][3] << " ]\n"
        << "         [ " << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ", " << mat[2][3] << " ]\n"
        << "         [ " << mat[3][0] << ", " << mat[3][1] << ", " << mat[3][2] << ", " << mat[3][3] << " ]";

    return oss.str();
}

std::string to_string(float flt)
{
    std::ostringstream oss;
    oss << flt;
    return oss.str();
}
std::string to_string(int Int)
{
    std::ostringstream oss;
    oss << Int;
    return oss.str();
}

std::string to_string(GLuint Int)
{
    std::ostringstream oss;
    oss << Int;
    return oss.str();
}

/*
enum primitive_print_t : uint8_t
{
    NONE = 0,
    FLOAT = 1,
    VEC2 = 2,
    VEC3 = 3,
    QUAT = 4,
    MAT4  = 5,
    INT = 6,
    STRING = 7,
    ARRAY = 128 // bit flag, if enabled, cast to a std::vector<primitive_print_t - 127>* and use it as such. It will give you the size, where it becomes possible to print arrays
};
struct member_reference
{
    primitive_print_t print_type;
    size_t offset;
    std::string get_string(void* object_start)
    {
        // offset by some number of bytes
        void* primitive_start = object_start + offset;

        // Depending on the print type, return the correct string
        switch (print_type)
        {
        case NONE:
            return "";
            break;

        case FLOAT:
            return to_string(*(float*)primitive_start);
            break;

        case VEC2:
            return to_string(*(glm::vec2*)primitive_start);
            break;

        case VEC3:
            return to_string(*(glm::vec3*)primitive_start);
            break;

        case QUAT:
            return to_string(*(glm::quat*)primitive_start);
            break;

        case MAT4:
            return to_string(*(glm::mat4*)primitive_start);

            break;
        case INT:
            return to_string(*(int*)primitive_start);
            break;

        case STRING:
            return *(std::string*)primitive_start;
            break;
        case ARRAY:
            return "arrays are not yet implemented..."
                break;
        default:
            return "ERROR: INVALID PRIMITIVE TYPE";
            break;
        }
    }
};*/