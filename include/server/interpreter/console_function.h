#pragma once
#include <include/utils/utility_types.h>

struct console_message;
class Token;
class ConsoleInterpreter;

using console_fn = func_ptr_t<void, console_message&, ConsoleInterpreter&, std::span<Token>>;