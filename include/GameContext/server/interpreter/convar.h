#pragma once
#include <include/GameContext/server/interpreter/InterpreterToken.h>
#include <cassert>



struct Convar
{
	void* target;
	token_type_t type;
    bool visible = 1;
    bool readonly = 0;
    bool is_command = 0;
    


    // Templated setter
    template <typename T>
    bool set(const T& val) {
        if (deduce_token_type<T>() != type)
        {
            return 1;
        }
        T* typed_target = (T*)target;
        *typed_target = val;

        // Allocate new memory and copy the target
        //target = new T(val);
        //type = deduce_token_type<T>();
        return 0;
    }

    template <typename T>
    bool is_type()
    {
        return type == deduce_token_type<T>();
    }

    // Templated getter
    template <typename T>
    T& get() const {
        assert(target != nullptr && "Attempting to get a target from an uninitialized convar.");
        assert(type == deduce_token_type<T>() && "Type mismatch in convar::get.");
        return *reinterpret_cast<T*>(target);
    }
    void set_command(void* ptr)
    {
        //target = 
    }


//private:
    // Helper to deduce the token type from the template type
    template <typename T>
    static token_type_t deduce_token_type() {
        
        if constexpr (std::is_same_v<T, int>) {
            return TYPE_INTEGER;
        }
        else if constexpr (std::is_same_v<T, float>) {
            return TYPE_FLOAT;
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            return TYPE_STRING;
        }
        else if constexpr (std::is_same_v<T, console_fn>) {
            return TYPE_CONSOLE_FN;
        }
        else if constexpr (std::is_same_v<T, glm::vec3>) {
            return TYPE_VEC3;
        }
        else {
            return TOKEN_UNKNOWN;
        }
    }

};