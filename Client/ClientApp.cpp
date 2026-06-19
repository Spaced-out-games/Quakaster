// setup
#include <platform/Quakaster.h>
#include "entrypoint.h"
#include "core.h"


// standard library
//#include <filesystem>
#include <iostream>
#include <vector>
#include <memory>

// platform
#include "platform/Application.h"
#include "platform/Window.h"



class ClientApp : public qk::Application {




	void init(int argc, char** argv) override {






	}

	void run() override {

	}

	void shutdown() override {
	}

};

std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<ClientApp>();
}