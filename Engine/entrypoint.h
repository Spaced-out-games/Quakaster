#pragma once
#include "core/core/Application.h"
#include <memory>

namespace qk {
	extern std::unique_ptr<qk::core::Application> create_application(int argc, char** argv);
}


int main(int argc, char** argv) {
    auto app = qk::create_application(argc, argv);

    app->init(argc, argv);

    while (app->status() == 0)
        app->run();

    return app->status();
}