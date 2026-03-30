/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/core/Application.cpp
/// Purpose: Handles Application initialization and management
/// **************************************************************************************************
#include "Application.h"
#include "spdlog/spdlog.h"


namespace qk::core {
	Application::Application() {
		spdlog::info("Application ctor called at {:#x}", (uintptr_t)this);
	}

	Application::~Application() {
		spdlog::info("Application dtor called at {:#x}", (uintptr_t)this);

	}
	int Application::status() {
		return m_Status;
	}

	void Application::set_status(int new_status) {
		m_Status = new_status;
	}

	void Application::init(int argc, char** argv) {
		spdlog::info("Application initialized at {:#x}", (uintptr_t)this);
	}

	void Application::close() {}
}