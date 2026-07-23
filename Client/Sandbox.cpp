#include "Sandbox.h"
using namespace qk::media;

using namespace qk::gfx::rhi;

std::vector<Vertex> g_Vertices = {
	{-1.0f,  1.0f, 0.0f}, // top-left
	{-1.0f, -1.0f, 0.0f}, // bottom-left
	{ 1.0f,  1.0f, 0.0f}, // top-right
	{ 1.0f, -1.0f, 0.0f}, // bottom-right
};
glm::vec3 tint = { 0.5, 0.25, 0.0 };

#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (53).png"

#define TEST_PIPELINE_PATH "assets/pipeline.jsonc"


void Sandbox::init(int argc, char** argv) {
	init_subsystems();
	qk::gfx::rhi::init(m_Window);

	// load pipelines and descriptor sets
	const auto pipeline_json = loadJSON(TEST_PIPELINE_PATH);
	loadPipelinesFromJSON(pipeline_json, pipelines, sets);

	// initialize vertex buffer
	hVertexBuffer = VertexBuffer_ctor(BufferUsage::StaticDraw);
	VertexBuffer_upload(hVertexBuffer, g_Vertices);


	// Descriptor set #1 - mock globals (just the texture)
	DescriptorSet_recipe globals;
	globals.emplace_back(EDescriptorType::Texture);
	hGlobals = DescriptorSet_ctor(globals);

	// load the image
	qk::media::Image img;
	qk::media::load_image(img, TEST_IMAGE_PATH);

	// create the texture
	auto tex_result = Texture_ctor(TEX_2D);
	assert(tex_result);
	hTex = *tex_result;
	Texture_upload_2D(hTex, img.data(), img.width(), img.height(), img.channels());

	// Add to globals descriptor set
	DescriptorSet_setResource(hGlobals, 0, hTex);


	// Descriptor set #2 - mock material (just an offset)
	DescriptorSet_recipe material;
	material.emplace_back(EDescriptorType::UniformBuffer);
	hMaterial = DescriptorSet_ctor(material);



	// make the uniform buffer and add it
	hUniformBuffer = UniformBuffer_ctor();
	UniformBuffer_upload(hUniformBuffer, &tint, 3, sizeof(float));
	DescriptorSet_setResource(hMaterial, 0, hUniformBuffer);




	// initialize pipeline
	hPipeline = Pipeline_ctor(pipelines["PBRPipeline"]);

	// make the commandbuffer
	CommandBuffer_recipe cmdBuf;
	cmdBuf
		.bindPipeline(hPipeline)
		.bindDescriptorSet(hGlobals, 0)
		.bindDescriptorSet(hMaterial, 1)
		.bindVertexBuffer(hVertexBuffer, 0, 0, 12)
		.drawArrays(0, 3);

	hCmdBuf = CommandBuffer_ctor(cmdBuf);


	//hCmdBuf

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

	CommandBuffer_execute(hCmdBuf);

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