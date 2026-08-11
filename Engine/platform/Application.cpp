/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/core/Application.cpp
/// Purpose: Handles Application initialization and management
/// **************************************************************************************************
#include "Application.h"
#include <spdlog/spdlog.h>


namespace qk {
	Application::Application() {
		spdlog::info("Application@{}: ctor called", (const void*)this);
	}

	Application::~Application() {
		spdlog::info("Application@{}: dtor called", (const void*)this);


	}
	int Application::status() {
		return m_Status;
	}

	void Application::setStatus(int new_status) {
		m_Status = new_status;
	}

	void Application::init(int argc, char** argv) {
		spdlog::info("Application@{}: initialized", (const void*)this);

	}

	void Application::shutdown() {}
}