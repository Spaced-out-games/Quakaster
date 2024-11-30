#pragma once
#include "glm_master.h"
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <typeinfo>

using member_lookup_function = std::string(*)(const std::string&);

enum primitive_t : uint8_t
{
    NONE_T = 0,
    FLOAT_T = 1,
    VEC2_T = 2,
    VEC3_T = 3,
    QUAT_T = 4,
    STRING_T = 5,
    PTR_T = 6,
    MAT4_T = 16
};

struct member_reference
{
    size_t offset;        // Offset from the start of the object
    primitive_t primitive; // Type of the primitive, used to decide how to parse it
};

class Inspectible
{
protected:
    // Holds member offsets and types for each member name
    std::unordered_map<std::string, member_reference> member_lookup_table;

public:
    // Adds a member to the lookup table
    void add_member(const std::string& name, size_t offset, primitive_t type)
    {
        member_lookup_table[name] = { offset, type };
    }

    // Retrieves a member's value as a string based on its name
    virtual std::string get_member(const std::string& member_name)
    {
        // Check if the member exists in the lookup table
        auto it = member_lookup_table.find(member_name);
        if (it == member_lookup_table.end()) {
            return "Member not found";
        }

        // Get the offset and type
        const member_reference& ref = it->second;
        const char* base_ptr = reinterpret_cast<const char*>(this) + ref.offset;

        // Convert the member to a string based on its type
        std::ostringstream oss;
        switch (ref.primitive) {
        case FLOAT_T:
            oss << *reinterpret_cast<const float*>(base_ptr);
            break;
        case VEC2_T:
        {
            const glm::vec2& vec = *reinterpret_cast<const glm::vec2*>(base_ptr);
            oss << "(" << vec.x << ", " << vec.y << ")";
        }
        break;
        case VEC3_T:
        {
            const glm::vec3& vec = *reinterpret_cast<const glm::vec3*>(base_ptr);
            oss << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        }
        break;
        case QUAT_T:
        {
            const glm::quat& quat = *reinterpret_cast<const glm::quat*>(base_ptr);
            oss << "(" << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z << ")";
        }
        break;
        case STRING_T:
            oss << *reinterpret_cast<const std::string*>(base_ptr);
            break;
        case PTR_T:
            oss << reinterpret_cast<const void*>(*reinterpret_cast<const uintptr_t*>(base_ptr));
            break;
        case MAT4_T:
        {
            const glm::mat4& mat = *reinterpret_cast<const glm::mat4*>(base_ptr);
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    oss << mat[i][j] << (j < 3 ? ", " : "");
                }
                oss << (i < 3 ? "; " : "");
            }
        }
        break;
        default:
            oss << "Unsupported type";
            break;
        }
        return oss.str();
    }
    // Gets the string representation of the type
    virtual std::string to_string()
    {
        return "";
    }

    // Array access by key (member name)
    std::string operator[](const std::string& member_name) {
        return get_member(member_name);
    }

    // Array access by index
    std::string operator[](size_t index) {
        if (index >= member_lookup_table.size()) {
            return "Index out of range";
        }

        auto it = member_lookup_table.begin();
        std::advance(it, index); // Move the iterator to the desired index
        return get_member(it->first); // Return the member value by its name
    }

    // Array access by type
    template<typename T>
    std::string operator[](T) {
        std::string type_name = typeid(T).name(); // Get the type name
        return get_member(type_name); // Access member by type name
    }

    // Provides a vector of member names for index-based access
    std::vector<std::string> get_member_names() const {
        std::vector<std::string> names;
        for (const auto& pair : member_lookup_table) {
            names.push_back(pair.first);
        }
        return names;
    }
};
