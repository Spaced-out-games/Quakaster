#pragma once
#include "application.h"
#include "controller.h"
#include "UICore.h"

// Game application. Inherits from Application
struct gameApp : public Application
{
    // Bootstrap function implementation as a static function
    static void bootstrap_impl(Application& app);

    // Tick function implementation as a static function
    static void tick_impl(Application& app);

    // Draw function implementation as a static function
    static void draw_impl(Application& app);

    // Constructor creates the application and assigns function pointers
    gameApp() : Application()
    {
        tick_fn = tick_impl;
        bootstrap_fn = bootstrap_impl;
        draw_fn = draw_impl;
    }
};

// Ensure proper initialization if Entity requires it
static Entity textEntity;  // Make sure this is correctly defined

// Bootstrap implementation
void gameApp::bootstrap_impl(Application& app)
{
    // Sets the scene: somewhere, some place, some time.
    Entity::setCurrentScene(app.scene);

    // Creates an entity, adds a console component
    textEntity.add_component<glm::vec3>();  // Ensure this is valid

    // Adds event listeners for console events
    ADD_EVENT_LISTENER(app.scene.dispatcher, console_log_request, console_log_request::defaultEventListener);
    ADD_EVENT_LISTENER(app.scene.dispatcher, ConsoleCommandLineEvent, ConsoleCommandLineEvent::default_eventListener);

    // Sets the background color to black
    glClearColor(0, 0, 0, 0);
    // Additional UI setup can go here if needed
}

// Tick function implementation
void gameApp::tick_impl(Application& app)
{
    // Creates a static event. Just to make SDL happy.
    SDL_Event event;

    static basic_controller inputController(app, textEntity);  // Ensure this is properly defined

    // A console log event. When it's sent, the system will print the text to the console
    console_log_request request("Test event", app.console);

    inputController.dispatchIOEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear buffers

    // Draw ImGui console
    app.draw();

    // Tick the scene
    app.scene.tick();
}

// Draw function implementation
void gameApp::draw_impl(Application& app)
{
    app.window.beginImGuiFrame();  // Begin ImGui frame
    app.console.draw();  // Draw console
    app.window.endImGuiFrame();  // End ImGui frame
}
