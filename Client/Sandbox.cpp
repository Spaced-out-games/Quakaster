#include "Sandbox.h"

using namespace qk::media;



std::vector<Vertex> g_Vertices = {
	{-1.0f,  1.0f, 0.0f}, // top-left
	{-1.0f, -1.0f, 0.0f}, // bottom-left
	{ 1.0f,  1.0f, 0.0f}, // top-right
	{ 1.0f, -1.0f, 0.0f}, // bottom-right
};

#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (53).png"


void Sandbox::init(int argc, char** argv) {
	init_subsystems();


	




}

void Sandbox::run() {

	// populates event m_EventQueue
	m_Window.pollEvents();
	// flushes it
	m_Layers.propagate_events();

	m_Layers.render();


	// presents offscreen image
	m_Window.swapBuffers();


	// updates when told to by the m_Window.
	setStatus(m_Window.shouldClose());

}

void Sandbox::shutdown() {}


std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<Sandbox>();
}