#include "Quakaster.h"
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

namespace qk {

    static void glfw_error_callback(int error, const char* description) {
        __debugbreak();
        spdlog::error("GLFW Error {}: {}", error, description);
    }

    bool startup() {

        glfwSetErrorCallback(glfw_error_callback);

        if (!glfwInit())
            return false;

        spdlog::info("Initialized glfw");


        return true;
    }
    void shutdown() {
        spdlog::info("Terminated glfw");
        glfwTerminate();
    }
}