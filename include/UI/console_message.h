#pragma once
#include <string>
#include "console_color.h"


struct console_message {
    console_color color;
    std::string message;

    console_message(std::string message, console_color color = console_color::DEFAULT_TEXT)
        : color(color), message(message) {}

    console_message& operator+=(const std::string& append_string) {
        message += append_string;
        return *this; // Return the modified object
    }
};
