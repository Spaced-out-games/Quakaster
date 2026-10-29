#pragma once
#include "platform/Application.h"
#include <memory>
#include <format>


namespace qk {
    extern std::unique_ptr<Application> create_application(int argc, char** argv);
}


int main(int argc, char** argv) {

    spdlog::info("------------------------------------------------------------------------------------------------------------");
    spdlog::info("Quakaster-{} Version {}.{}.{}: {} ({})",
        QK_GIT_BRANCH,
        QK_VERSION_MAJOR,
        QK_VERSION_MINOR,
        QK_VERSION_PATCH,
        QK_BUILD_CONFIGURATION,
        QK_GIT_HASH
    );
    spdlog::info("{}", QK_VERSION_DESCRIPTION);
    spdlog::info("------------------------------------------------------------------------------------------------------------");



    auto app = qk::create_application(argc, argv);

    app->init(argc, argv);

    while (app->status() == 0)
        app->run();

    app->shutdown();

    return app->status();
}