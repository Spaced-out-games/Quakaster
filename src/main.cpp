#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <include/entt.hpp>
#include <iostream>
#include "include/window.h"

#include "include/entity.h"
#include "include/textComponent.h"

#include "include/application.h"

uint64_t num_ticks = 0;

int main() {
    Application app;

    Entity::setCurrentScene(app.scene);


    Entity textEntity;
    textEntity.add_component<textComponent>("Test");
    app.scene.addThink(thinkScript(textComponent::think));

    SDL_Event event;



    glClearColor(255, 0, 0, 0);
    while (app.window.is_running()) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event); // Process events for ImGui
            if (event.type == SDL_QUIT) {
                app.window.quit();
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        



        app.window.beginImGuiFrame();
        app.console.draw();
        app.scene.tick();
        app.window.endImGuiFrame();

    }
    
    return 0;
}

