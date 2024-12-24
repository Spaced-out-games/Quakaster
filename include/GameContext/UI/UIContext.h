#pragma once
#include "ConsoleUI.h"

struct UIContext
{
	// Whether UI is selected
	uint8_t activeElement;

	// the console interface
	ConsoleUI console;

	// pauses to the console UI element
	void pause()
	{

		activeElement = 1;
	}

};