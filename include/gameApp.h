#pragma once
#include "application.h"
#include "controller.h"
#include "UICore.h"





// Game application. Inherits from Application
struct gameApp : public Application
{
	UI::IWidgetComponent* button = nullptr;
	UI::IWidgetComponent* label = nullptr;
	// Bootstrap function implementation as a static function
	static void bootstrap_impl(Application& app);

	// Tick function implementation as a static function
	static void tick_impl(Application& app);


	static void draw_impl(Application& app);

	// Constructor creates the application and assigns function pointers
	gameApp(): Application()
	{
		tick = tick_impl;
		bootstrap = bootstrap_impl;
	}
};

//static UI::UIWidget widget;

// Bootstrap implementation
void gameApp::bootstrap_impl(Application& app)
{
	// Sets the scene: somewhere someplace somewhen.
	Entity::setCurrentScene(app.scene);

	// Creates an entity, add a consoleComponent so it can log to the screen
	static Entity textEntity;
	textEntity.add_component<consoleComponent>("consoleComponent tick", app.console);

	// Adds a think script
	//app.scene.addThink(consoleComponent::think, 5);

	// Adds an event listener
	ADD_EVENT_LISTENER(app.scene.dispatcher, console_log_request, console_log_request::defaultEventListener);
	ADD_EVENT_LISTENER(app.scene.dispatcher, ConsoleCommandLineEvent, ConsoleCommandLineEvent::default_eventListener);


	// Sets the background color to black
	glClearColor(0, 0, 0, 0);
	//app.cast<gameApp>().button = new ImGuiButton(app.scene.dispatcher, "Button");
	//app.cast<gameApp>().label  = new ImGuiLabel(app.scene.dispatcher, "Label");

	ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Arial.ttf", 24.0f);

	//widget.add_component<UI::Button>(app.scene.dispatcher, "Button test");
	//widget.add_component<UI::Checkbox>("Test", false);

	#ifdef _DEBUG
		
	#endif

	
}



void gameApp::tick_impl(Application& app)
{
	// Creates a static event. Just to make SDL happy.
	SDL_Event event;
	static Controller inputController(app);

	
	// A console log event. When it's sent, the system will print the text to the console
	console_log_request request("Test event", app.console);


	// Poll SDL events
	/*
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);  // Process events for ImGui
		if (event.type == SDL_QUIT) {
			app.window.quit();
		}
	}*/

	inputController.processInput();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Fire the event (passed by reference)
	//EVENT_FIRE(app.scene.dispatcher, console_log_request, request);

	// Draw imGui console
	app.window.beginImGuiFrame();
	app.console.draw();
	app.window.endImGuiFrame();

	// Tick the scene
	app.scene.tick();

}

void gameApp::draw_impl(Application& app)
{

}