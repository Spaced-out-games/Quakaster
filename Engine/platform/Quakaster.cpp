#include "Quakaster.h"
#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

namespace qk {

    static void glfw_error_callback(int error, const char* description) {

        QK_ASSERT(error == 0, std::format("GLFW Error {}: {}", error, description));

    }

    bool startup() {

        glfwSetErrorCallback(glfw_error_callback);

        if (!glfwInit())
            return false;

        spdlog::info("Initialized glfw");

        //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        return true;
    }
    void shutdown() {
        spdlog::info("Terminated glfw");
       // glfwTerminate();
    }
}