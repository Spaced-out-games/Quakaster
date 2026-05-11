#include "Quakaster.h"
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

namespace qk {

    static void glfw_error_callback(int error, const char* description) {
        //QK_DEBUG_BREAK();
        spdlog::error("GLFW Error {}: {}", error, description);
        QK_DEBUG_BREAK();
    }

    bool startup() {

        glfwSetErrorCallback(glfw_error_callback);

        if (!glfwInit())
            return false;

        spdlog::info("Initialized glfw");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        
        return true;
    }
    void shutdown() {
        spdlog::info("Terminated glfw");
        glfwTerminate();
    }
}