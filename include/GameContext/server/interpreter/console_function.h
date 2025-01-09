#pragma once
#include <include/GameContext/utils/utility_types.h>

struct console_message;
class Token;
class ConsoleInterpreter;

using console_fn = func_ptr_t<void, console_message&, ConsoleInterpreter&, std::span<Token>>;