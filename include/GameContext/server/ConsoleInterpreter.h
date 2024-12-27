#pragma once
#include <string>

struct ConsoleUI;

struct ConsoleInterpreter
{
	bool execute_command(std::string& content, ConsoleUI* caller);
};