#pragma once

#ifndef _DEBUG
#pragma message("Warning: 'Inspector.h' included in Release mode. Including it may harm performance and security")
#endif

#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>
#include <stdexcept>

// Define the function pointer type for hooks
using hook_t = std::function<std::string(void*)>;

class Inspector {
private:
    void* owner;
    std::unordered_map<std::string, hook_t> hooks;

public:
    Inspector(void* owner) : owner(owner) {}

    void register_hook(const std::string& member_name, hook_t hook) {
        hooks[member_name] = hook;
    }

    std::string operator[](const std::string& member_name) const {
        auto it = hooks.find(member_name);
        if (it != hooks.end()) {
            return it->second(owner);
        }
        return "Member not found";
    }
};
