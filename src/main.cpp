#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <include/entt.hpp>
#include <iostream>
#include "include/window.h"
#include "include/Console.h"
#include "include/scene.h"
#include "include/entity.h"
#include "include/textComponent.h"



uint64_t num_ticks = 0;

int main() {
    Scene scene;
    std::cout << "actually print dammit";
    Entity::setCurrentScene(scene);
    Entity textEntity;
    
    //textComponent tc("test");
    textEntity.add_component<textComponent>("Test");
    scene.addThink(thinkScript(textComponent::think));

    Window window(1920, 1080);
    SDL_Event event;



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


    glClearColor(255, 0, 0, 0);
    while (window.is_running()) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event); // Process events for ImGui
            if (event.type == SDL_QUIT) {
                window.quit();
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        



        window.beginImGuiFrame();
        console.draw();
        scene.tick();
        window.endImGuiFrame();

    }
    


    return 0;
}

