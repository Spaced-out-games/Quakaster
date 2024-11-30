#pragma once
#include <typeinfo> // For std::type_info
#include <cstddef>  // For std::size_t
#include <string_view>
#include <stdexcept> // For std::out_of_range
#include <cstdio>    // For std::snprintf
#include <type_traits> // For std::is_same


using small_size_t = uint16_t;

// Command for an entity to execute
struct ent_command {
    ent_command(uint16_t command_ID = 0, small_size_t size = 0, uint8_t* ptr = nullptr)
        : command_ID(command_ID), buffer_size(size) {
        if (ptr == nullptr) {
            data = new uint8_t[size]; // Allocate memory if no pointer is provided
        }
        else {
            data = ptr; // Use the provided pointer
        }
    }

    template <typename T>
    T read_next(small_size_t& offset) {
        if (offset + sizeof(T) > buffer_size) {
            throw std::out_of_range("Attempted to read past the end of command data.");
        }

        T out = *reinterpret_cast<T*>(data + offset);
        offset += sizeof(T);
        return out;
    }

    void reset(small_size_t& offset) { offset = 0; }

    template <typename T>
    void write_next(const T& value, small_size_t& offset) {
        if (offset + sizeof(T) > buffer_size) {
            throw std::out_of_range("Attempted to write past the end of command data.");
        }

        *reinterpret_cast<T*>(data + offset) = value;
        offset += sizeof(T);
    }

    small_size_t size() const {
        return buffer_size;
    }

    uint16_t command_code() { return command_ID; }

    uint16_t command_ID = 0;
    small_size_t buffer_size = 0;
    uint8_t* data = nullptr;
};

template <typename target_ent_t>
struct sv_command {
    uint16_t index;
    ent_command command;

    void exec() {};
};

// Inheritor_t is used to get information about derived types. So you should have something like
// `class ent_derived: public ent<ent_derived>`  in the declaration. It allows functions like get_name() to work without your input.
// If you want a standalone `ent` entity, just pass `void` as the inheritor_t to denote you are working with a pure `ent`

struct ent {


    ent() = default;                       // Default constructor
    virtual ~ent() = default;              // Virtual destructor

    // Delete copy constructor and copy assignment operator
    ent(const ent&) = default;
    // Disable copying
    ent& operator=(const ent&) = default;   // Disable copy assignment


    // Default move constructor and move assignment operator
    ent(ent&&) = default;                  // Enable moving
    ent& operator=(ent&&) = default;       // Enable move assignment

    // Pure virtual functions

    virtual void init() = 0;
    virtual void tick() = 0;
    virtual void exec(const ent_command& command) = 0;
    // All derived types need this method, but it has to be static so it's publically accessible
    static const char* get_name() { return "ent"; }
    virtual std::string get_member(std::string member_name) = 0;// { return "Not implemented"; }
    
    // Functions with these signatures should exist in templated types
    // static void parallel_tick(void* array_start, size_t number_of_elements);
    // static void parallel_init(void* array_start, size_t number_of_elements);

    // Given a string to referring to a member variable (eg, 'position'), output a console-printable string of that member (eg, `(1.0,3.45,5.2)`)
    //virtual std::string get_member_by_name(const std::string& member_name) = 0;

    

private:

};
