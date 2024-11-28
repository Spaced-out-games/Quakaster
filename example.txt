#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <include/entt.hpp>
#include <iostream>
#include "include/window.h"

#include "include/entity.h"
#include "include/consoleComponent.h"
#include "include/textComponent.h"


#include "include/application.h"

uint64_t num_ticks = 0;

struct MyEvent {
    Entity* target;
    glm::vec3 velocity;

    MyEvent(Entity* target, float x, float y, float z)
        : target(target), velocity(glm::vec3(x, y, z)) {}
};
struct Position
{
    glm::vec3 position;
    Position(float x, float y, float z) : position(x, y, z) {}
};

void handleMyEvent(MyEvent& event) {
    glm::vec3& ent_position = event.target->getComponent<Position>().position;
    ent_position += event.velocity;
    std::cout << "(" << ent_position.x << ", " << ent_position.y << ", " << ent_position.z << ")\n";
}
int main() {
    Application app;

    // Set the current scene for the entities
    Entity::setCurrentScene(app.scene);

    // Create an entity and add a Position component
    Entity textEntity;
    textEntity.add_component<Position>(0.0, 0.0, 0.0);

    // Register the event listener for MyEvent
    ADD_EVENT_LISTENER(app.scene.dispatcher, MyEvent, handleMyEvent);

    // SDL Event handling
    SDL_Event event;
    MyEvent velocityEvent(&textEntity, 0, 0, 0.1);

    glClearColor(255, 0, 0, 0);
    while (app.window.is_running()) {
        // Poll SDL events
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);  // Process events for ImGui
            if (event.type == SDL_QUIT) {
                app.window.quit();
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Fire the event (passed by reference)
        EVENT_FIRE(app.scene.dispatcher, MyEvent, velocityEvent);  // Correctly fire the event

        app.window.beginImGuiFrame();
        app.console.draw();
        app.scene.tick();
        app.window.endImGuiFrame();
    }

    return 0;
}