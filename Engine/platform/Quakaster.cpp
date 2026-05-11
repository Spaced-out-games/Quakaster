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

        //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        return true;
    }
    void shutdown() {
        spdlog::info("Terminated glfw");
       // glfwTerminate();
    }
}