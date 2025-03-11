#pragma once

enum class console_color : uint8_t
{
    WHITE = 0,
    DEFAULT_TEXT = WHITE,
    RED = 1,
    DEFAULT_ERROR_CRITICAL = RED,
    GREEN = 2,
    DEFAULT_OKAY = GREEN,
    YELLOW = 3,
    DEFAULT_WARNING = YELLOW,
    ORANGE = 4,
    DEFAULT_WARNING_SEVERE = ORANGE
};