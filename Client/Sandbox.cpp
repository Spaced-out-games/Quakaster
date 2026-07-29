#include "Sandbox.h"
#include <gfx/gfx.h>
#include <fnv1a.h>
using namespace qk::media;
using qk::operator""_sh;
using namespace qk::gfx;

#include "level/LevelLoader.h"

using ComponentLoader = qk::IComponentLoaderMap<str, int, float>;









std::vector<Vertex> g_Vertices = {
	{-1.0f,  1.0f, 0.0f}, // top-left
	{-1.0f, -1.0f, 0.0f}, // bottom-left
	{ 1.0f,  1.0f, 0.0f}, // top-right
	{ 1.0f, -1.0f, 0.0f}, // bottom-right
};
glm::vec3 tint = { 0.5, 0.25, 0.0 };

#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (53).png"

#define TEST_PIPELINE_PATH "assets/pipeline.jsonc"


entt::registry reg;
ComponentLoader ld;
void Sandbox::init(int argc, char** argv) {
	init_subsystems();
	rhi::init(m_Window);

	
	

	// load pipelines and descriptor sets
	const auto pipeline_json = loadJSON(TEST_PIPELINE_PATH);
	loadPipelinesFromJSON(pipeline_json, pipelines, sets);

	// initialize vertex buffer amd register
	hVertexBuffer = VertexBuffer_ctor(BufferUsage::StaticDraw);
	GBuffers.insert("VertexBuffer"_sh, hVertexBuffer);
	VertexBuffer_upload(hVertexBuffer, g_Vertices);

	// Descriptor set #1 - mock globals (just the texture) and register DS
	DescriptorSet_recipe globals;
	globals.emplace_back(EDescriptorType::Texture);
	hGlobals = DescriptorSet_ctor(globals);
	DescriptorSets.insert("hGlobals"_sh, hGlobals);

	

	// load the image
	qk::media::Image img;
	qk::media::load_image(img, TEST_IMAGE_PATH);

	// create the texture
	auto tex_result = Texture_ctor(TEX_2D);
	assert(tex_result);
	hTex = *tex_result;
	Textures.insert(qk::string_hash{ TEST_IMAGE_PATH }, hTex);

	Texture_upload_2D(hTex, img.data(), img.width(), img.height(), img.channels());

	// Add to globals descriptor set
	DescriptorSet_setResource(hGlobals, 0, Textures.peek(qk::string_hash{ TEST_IMAGE_PATH }));

	// Descriptor set #2 - mock material (just an offset)
	DescriptorSet_recipe material;
	material.emplace_back(EDescriptorType::UniformBuffer);
	hMaterial = DescriptorSet_ctor(material);
	DescriptorSets.insert("hMaterial"_sh, hMaterial);

	



	// make the uniform buffer and add it
	hUniformBuffer = UniformBuffer_ctor();
	GBuffers.insert("hUniformBuffer"_sh, hUniformBuffer);

	UniformBuffer_upload(hUniformBuffer, &tint, 3, sizeof(float));
	DescriptorSet_setResource(hMaterial, 0, hUniformBuffer);



	// initialize pipeline
	hPipeline = Pipeline_ctor(pipelines["PBRPipeline"]);
	Pipelines.insert("PBRPipeline"_sh, hPipeline);

	// make the commandbuffer
	CommandBuffer_recipe cmdBuf;
	cmdBuf
		.bindPipeline(hPipeline)
		.bindDescriptorSet(hGlobals, 0)
		.bindDescriptorSet(hMaterial, 1)
		.bindVertexBuffer(hVertexBuffer, 0, 0, 12)
		.drawArrays(0, 4);

	hCmdBuf = CommandBuffer_ctor(cmdBuf);
	CommandBuffers.insert(0, hCmdBuf);



	//hCmdBuf

}



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

	destroy_all_resources();

}


std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<Sandbox>();
}