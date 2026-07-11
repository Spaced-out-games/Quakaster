#include "Sandbox.h"
using namespace qk::media;

using namespace qk::gfx::rhi;

std::vector<Vertex> g_Vertices = {
	{-1.0f,  1.0f, 0.0f}, // top-left
	{-1.0f, -1.0f, 0.0f}, // bottom-left
	{ 1.0f,  1.0f, 0.0f}, // top-right
	{ 1.0f, -1.0f, 0.0f}, // bottom-right
};

#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (53).png"

#define TEST_PIPELINE_PATH "assets/pipeline.jsonc"



void Sandbox::init(int argc, char** argv) {
	init_subsystems();
	qk::gfx::rhi::init(m_Window);
	const auto pipeline_json = loadJSON(TEST_PIPELINE_PATH);
	std::unordered_map<str, Pipeline_recipe> pipelines;
	std::unordered_map<str, DescriptorSetLayout_recipe> sets;
	loadPipelinesFromJSON(pipeline_json, pipelines, sets);

	hVertexBuffer = VertexBuffer_ctor(BufferUsage::StaticDraw);
	VertexBuffer_upload(hVertexBuffer, g_Vertices);

	hPipeline = Pipeline_ctor(pipelines["PBRPipeline"]);

}

// TODO: It currently renders and we now have decoupled pipeline creation from vertexbuffer binding. It works, but is incredibly
// fragile, for the sake of testing. The vertex buffer layout strides are cached in Pipeline_impl and is ready for CommandBuffer
// to link it and use it when CmdBuf.bindVertexBuffer() is eventually called. For now, vertex buffer bind indices are generated
// from the VertexLayout's binding[] index, but each binding should be able to explicitly declare its bind slot, which will of
// course map to the pipeline's stride

void Sandbox::run() {

	// populates event m_EventQueue
	m_Window.pollEvents();
	// flushes it
	m_Layers.propagate_events();
	Pipeline_bind(hPipeline);
	VertexBuffer_bind(hVertexBuffer, 0, 0, 12);

	draw_test();
	m_Layers.render();


	// presents offscreen image
	m_Window.swapBuffers();


	// updates when told to by the m_Window.
	setStatus(m_Window.shouldClose());

}

void Sandbox::shutdown() {
	VertexBuffer_dtor(hVertexBuffer);
}


std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<Sandbox>();
}