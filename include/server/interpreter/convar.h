#pragma once
#include <string>
#include <cstring> // For strncpy
#include "glew_surrogate.h"
#include <unordered_map>
#include <stdexcept>

#define CONVAR_STRING_SIZE 128

using Convar_fn = func_ptr_t<void, std::stringstream&>;


enum Convar_type {
    NONE,
    INTEGER,
    FLOAT,
    STRING,
    GLUINT,
    GLENUM,
    FUNCTION // for things like "exit" or "echo" which are not simply convars
};



class Client;
class Server;





struct Convar {
    static inline std::unordered_map<std::string, Convar> convars; // private, ideally
    ~Convar() = default;
    
    template <typename T>
    Convar(const T& value, bool clientRead = true, bool clientWrite = false, bool serverRead = true, bool serverWrite = true) :
        m_ClientCanRead(clientRead), m_ClientCanWrite(clientWrite),
        m_ServerCanRead(serverRead), m_ServerCanWrite(serverWrite) {

        if constexpr (std::is_same_v<T, int>) {
            m_Type = INTEGER;
            m_Value.m_Int = value;
        }
        else if constexpr (std::is_same_v<T, float>) {
            m_Type = FLOAT;
            m_Value.m_Float = value;
        }
        else if constexpr (std::is_same_v<T, GLuint>) {
            m_Type = GLUINT;
            m_Value.m_GLuint = value;
        }
        else if constexpr (std::is_same_v<T, std::string>) { // Handling std::string
            m_Type = STRING;
            if (value.size() >= 128) {
                throw std::runtime_error("String too long for Convar (max 127 characters)");
            }
            std::strncpy(m_Value.m_String, value.c_str(), 127);
            m_Value.m_String[127] = '\0'; // Ensure null termination
        }
        else if constexpr (std::is_same_v<T, Convar_fn>) {
            m_Type = FUNCTION;
            m_Value.m_Function = value;
        }
        else {
            // Optionally handle unsupported types
            //static_assert(false, "Unsupported type for Convar");
            __debugbreak();
        }
    }
    Convar(bool clientRead = true, bool clientWrite = false, bool serverRead = true, bool serverWrite = true) : m_Type(NONE),
        m_ClientCanRead(clientRead), m_ClientCanWrite(clientWrite),
        m_ServerCanRead(serverRead), m_ServerCanWrite(serverWrite) {
    }

    // Convert to string representation
    std::string to_string() const {
        switch (m_Type) {
        case INTEGER: return std::to_string(m_Value.m_Int);
        case FLOAT: return std::to_string(m_Value.m_Float);
        case STRING: return std::string(m_Value.m_String);
        case GLUINT: return std::to_string(m_Value.m_GLuint);
        case GLENUM: return std::to_string(m_Value.m_GLenum);
        default: return "Unknown";
        }
    }

    // Permission accessors
    bool can_client_read() const { return m_ClientCanRead; }
    bool can_client_write() const { return m_ClientCanWrite; }
    bool can_server_read() const { return m_ServerCanRead; }
    bool can_server_write() const { return m_ServerCanWrite; }

    bool read_only() const { return (can_client_read() || can_server_read()) && (!can_client_write() && !can_server_write()); }

    template <typename T>
    T& as() {
        if constexpr (std::is_same_v<T, int>) {
            if (m_Type != INTEGER) throw std::runtime_error("Type mismatch: Expected int");
            return m_Value.m_Int;
        }
        else if constexpr (std::is_same_v<T, float>) {
            if (m_Type != FLOAT) throw std::runtime_error("Type mismatch: Expected float");
            return m_Value.m_Float;
        }
        else if constexpr (std::is_same_v<T, GLuint>) {
            if (m_Type != GLUINT) throw std::runtime_error("Type mismatch: Expected GLuint");
            return m_Value.m_GLuint;
        }
        else if constexpr (std::is_same_v<T, GLenum>) {
            if (m_Type != GLENUM) throw std::runtime_error("Type mismatch: Expected GLenum");
            return m_Value.m_GLenum;
        }
        else if constexpr (std::is_same_v<T, char[128]>) {
            if (m_Type != STRING) throw std::runtime_error("Type mismatch: Expected string");
            return m_Value.m_String;
        }
        else {
            throw std::runtime_error("Unsupported type");
        }
    }


    
    union {
        int m_Int;
        float m_Float;
        char m_String[CONVAR_STRING_SIZE]; // Fixed-size string storage
        GLuint m_GLuint;
        GLenum m_GLenum;
        Convar_fn m_Function;
    } m_Value;

    Convar_type type() { return m_Type; }

protected:
    friend class Client;
    friend class Server;



    template <typename T, typename ...Args>
    friend void add_convar(const std::string& name, const T& value, Args ...args);


    friend Convar& get_convar(const std::string& name);




    

    



    struct {
        unsigned int m_ClientCanRead : 1;
        unsigned int m_ClientCanWrite : 1;
        unsigned int m_ServerCanRead : 1;
        unsigned int m_ServerCanWrite : 1;

        // Whether or not servers need to propigate to clients
        unsigned int m_Replicated : 1;
    };


private:
    Convar_type m_Type;
    // Bitfield for permissions
    
    

    template <typename T>
    Convar& operator=(const T& value) {
        if (read_only()) throw std::runtime_error("Convar is read-only");

        if constexpr (std::is_same_v<T, int>) {
            m_Type = INTEGER;
            m_Value.m_Int = value;
        }
        else if constexpr (std::is_same_v<T, float>) {
            m_Type = FLOAT;
            m_Value.m_Float = value;
        }
        else if constexpr (std::is_same_v<T, GLuint>) {
            m_Type = GLUINT;
            m_Value.m_GLuint = value;
        }
        else if constexpr (std::is_same_v<T, GLenum>) {
            m_Type = GLENUM;
            m_Value.m_GLenum = value;
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            m_Type = STRING;
            if (value.size() >= CONVAR_STRING_SIZE) {
                throw std::runtime_error("String too long for Convar (max 127 characters)");
            }
            std::strncpy(m_Value.m_String, value.c_str(), CONVAR_STRING_SIZE-1);
            m_Value.m_String[CONVAR_STRING_SIZE-1] = '\0'; // Ensure null termination
        }
        else {
            throw std::runtime_error("Unsupported assignment type");
        }
        return *this;
    }

    
};

template <typename T, typename ...Args>
void add_convar(const std::string& name, const T& value, Args ...args) {

    if (Convar::convars.find(name) != Convar::convars.end()) {
        throw std::runtime_error("Convar already exists");
    }
    Convar::convars[name] = Convar(value, std::forward<Args>(args)...);
}

static Convar& get_convar(const std::string& name) {
    if (Convar::convars.find(name) != Convar::convars.end()) {
        if (Convar::convars[name].can_client_read()) {
            return Convar::convars[name];

        }
        else {
            throw std::runtime_error("No permission to read this convar");
        }
    }
    else { // can't find the convar
        throw std::runtime_error("Could not find convar");
    }
}

/*
enum Convar_type {
    NONE,
    INTEGER,
    FLOAT,
    STRING,
    GLUINT,
    GLENUM,
    FUNCTION // for things like "exit" or "echo" which are not simply convars
};
*/

void set_convar(Convar& convar, std::stringstream& ss) {

}


void run_command(const std::string& command) {
    // get the command name
    std::stringstream ss(command);
    std::string command_name;
    ss >> command_name;


    // look up the convar
    if (Convar::convars.find(command_name) != Convar::convars.end()) {
        Convar& console_variable = get_convar(command_name);
        std::string new_value;
        std::string temp_str;

        switch (console_variable.type())
        {
            case NONE:
                break; // don't do anything
            case INTEGER:
                int temp_int;
                if (ss >> temp_int) {
                    console_variable.m_Value.m_Int = temp_int;
                    new_value = std::to_string(temp_int);
                }
                break;
            case FLOAT:
                float temp_fl;
                if (ss >> temp_fl) {
                    console_variable.m_Value.m_Float = temp_fl;
                    new_value = std::to_string(temp_fl);
                }
                break;
            case STRING:
                // just memcpy
                
                if (ss >> temp_str) {
                    memcpy(console_variable.m_Value.m_String, temp_str.c_str(), CONVAR_STRING_SIZE);
                    // make sure it's null-terminated
                    console_variable.m_Value.m_String[CONVAR_STRING_SIZE - 1] = '\0';
                    new_value = '"';
                    new_value += temp_str.substr(0, std::min((size_t)CONVAR_STRING_SIZE, temp_str.size()));
                    new_value += '"';
                }
                break;
            case GLENUM: // just fall through, since they are the same type under the hood, for now
            case GLUINT:
                GLuint temp_gluint;
                if (ss >> temp_gluint) {
                    console_variable.m_Value.m_GLuint = temp_gluint;
                    new_value = std::to_string(temp_gluint);
                }
                break;
            case FUNCTION: // just pass the ostream
                console_variable.m_Value.m_Function(ss);
                DevMsg(std::string("Ran ") + command_name);
                return; // prevent it from hitting the DevMsg call below
            default:
                break;
        }
        DevMsg(std::string("Set convar \"") + command_name + "\" to " + new_value);

    }
    else {
        DevMsg(std::string("Convar \"") + command_name + std::string("\" does not exist."), console_color::RED);
    }
}