#include <core/core/Quakaster.h>
#include <core/core/Application.h>
#include <core/core/Window.h>

#include <core/io/EventQueue.h>
#include <core/io/LayerStack.h>
#include <core/io/Layer.h>

#include <math/geometry/Line.h>

#include <entrypoint.h>
#include <core.h>

using namespace qk::core;

struct ClientApplication : Application {
	qk::Line line;

	Window window;
	LayerStack evt_layerstack;
	EventQueue evt_queue;

	void init(int argc, char** argv) override;

	void run() override;

	void close() override;


};

std::unique_ptr<Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<ClientApplication>();
}