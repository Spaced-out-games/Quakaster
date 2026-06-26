#include "Sandbox.h"

using namespace qk::media;



std::vector<Vertex> g_Vertices = {
	{-1.0f,  1.0f, 0.0f}, // top-left
	{-1.0f, -1.0f, 0.0f}, // bottom-left
	{ 1.0f,  1.0f, 0.0f}, // top-right
	{ 1.0f, -1.0f, 0.0f}, // bottom-right
};

#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (53).png"


static rhi::HTexture hTex;

void Sandbox::init(int argc, char** argv) {
	init_subsystems();


	// Load shit from disk
	auto j = loadJSON("assets/pipeline.jsonc");
	auto layouts = VertexLayoutsFromJSON(j["layouts"]);
	auto materials = qk::media::MaterialsFromJSON(j["materials"]).value();
	auto sets = qk::media::DescriptorSetsFromJSON(j["descriptor_sets"]).value();



	// load texture
	qk::media::Image img;
	qk::media::load_image(img, TEST_IMAGE_PATH); 

	auto tex_res = rhi::Texture_ctor("uTex", rhi::TextureType::TEX_2D);
	hTex = tex_res.value();
	rhi::Texture_upload_2D(hTex, img.data(), img.width(), img.height(), img.channels());



	// load layouts
	if (!layouts) {
		spdlog::critical("{}", layouts.error());
		std::abort();
	}
	layouts.value()["Vertex"].label = "Vertex";
	auto hLayout = rhi::VertexLayout_ctor(layouts.value()["Vertex"]);


	// update the camera UBO
	g_Camera.view = glm::translate(g_Camera.view, { 1.0,1.0,0 });
	g_Camera.view = glm::scale(g_Camera.view, { 2,2,1 });


	// upload vertex data
	rhi::HVertexBuffer vbo = rhi::VertexBuffer_ctor("vbo", rhi::StaticDraw);
	rhi::VertexBuffer_upload(vbo, g_Vertices);

	
	// create the uniform buffer that's linked by the descriptor set.
	rhi::HUniformBuffer ubo = rhi::UniformBuffer_ctor("CameraData");



	rhi::UniformBuffer_upload(ubo, &g_Camera, 1, sizeof(g_Camera));


	materials["mat_solid"].set_recipe = sets["CameraDataDescriptorSet"];

	auto material = rhi::Material_ctor(
		materials["mat_solid"]
	);


	rhi::HVertexArray vao = rhi::VertexArray_ctor("tri_mesh");
	rhi::VertexArray_bind(vao);

	rhi::VertexBuffer_bind(vbo);

	rhi::VertexLayout_apply(hLayout);

	if (!material) {
		spdlog::error("{}", material.error());
		std::abort();
	}
	
	auto mat = material.value();

	rhi::CommandBuffer_recipe CmdBufDesc;
	CmdBufDesc
		.push(rhi::ClearCommand_desc{ 1.0,0.0,1.0,0.0, rhi::Color })
		.push(rhi::DrawArrays_desc{ mat, vao, rhi::TriangleStrip, 0, g_Vertices.size()});


	CmdBuf = rhi::CommandBuffer_ctor(CmdBufDesc);



	rhi::Texture_bind(hTex, 0);




}

void Sandbox::run() {

	// populates event m_EventQueue
	m_Window.pollEvents();
	// flushes it
	m_Layers.propagate_events();

	rhi::CommandBuffer_execute(CmdBuf);
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