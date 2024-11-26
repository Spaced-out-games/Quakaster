#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <include/scene.h>

// CameraComponent.h

struct CameraComponent {
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    Scene* target_scene = nullptr;
    GLuint framebuffer;  // Reference to the framebuffer for this camera

    CameraComponent(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspectRatio, float nearPlane, float farPlane)
        : position(position), target(target), up(up) {
        updateViewMatrix();
        updateProjectionMatrix(fov, aspectRatio, nearPlane, farPlane);
    }

    void updateViewMatrix() {
        view = glm::lookAt(position, target, up);
    }

    void updateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) {
        projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void setFramebuffer(GLuint framebuffer) {
        this->framebuffer = framebuffer;
    }

    void renderScene() {
        // Bind the framebuffer to render to it
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Here, you would render the scene as usual, passing the camera's view and projection matrices
        //glUseProgram(shaderProgram);  // Assume the shader program is set up

        //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Render the scene (objects, meshes, etc.)
        // For example, iterate through entities and render them...
        renderSceneEntities();
    }

    void renderSceneEntities() {
        // Iterate through entities and draw them (you can assume the entity system is in place)
    }
};
