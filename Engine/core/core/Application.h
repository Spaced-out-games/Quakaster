/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/core/Application.h
/// Purpose: Defines a basic interface for Quakaster applications
/// ************************************************************************************************** 


#pragma once
#include "../../core.h"

namespace qk::core {
	class QK_API Application {
		int m_Status = 0;

		public:
		Application();
		virtual ~Application();

		// forwards argc and argv to the derived type so they can handle their own custom initialization logic.
		virtual void init(int argc, char** argv);

		// Runs repetitively until m_Status != 0
		virtual void run() = 0;
		// Handles resource cleanup
		virtual void close();
		// Gets the status code of the Application
		int status();

		// Sets the status code of the application.
		void set_status(int new_status);
	};
}

