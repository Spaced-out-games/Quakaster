#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <include/entt.hpp>
#include <iostream>
#include "include/window.h"
#include "include/Console.h"
#include "include/TextureComponent.h"
#include "include/MeshComponent.h"
#include "include/ShaderComponent.h"
#include "include/entity.h"



uint64_t num_ticks = 0;

int main() {
    Scene scene;
    Window window(1920, 1080);
    SDL_Event event;

    Console c;
    c.AttachToScene(scene);


    ShaderPool shader_pool;
    ShaderComponent shader ("C:/Users/devin/Source/Repos/Quakaster/resources/shaders/default.vert", "C:/Users/devin/Source/Repos/Quakaster/resources/shaders/default.frag" , shader_pool );
    shader.use();

    Console console;



    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    

    glClearColor(255, 0, 0, 0);
    while (window.is_running()) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event); // Process events for ImGui
            if (event.type == SDL_QUIT) {
                window.quit();
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        glUseProgram(shader.programID);
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, window.renderTexture); // Bind the texture rendered to
        glDrawArrays(GL_TRIANGLES, 0, 6);

        window.beginImGuiFrame();
        console.draw();
        window.endImGuiFrame();

        //window.swapBuffers();
    }
    


    return 0;
}

