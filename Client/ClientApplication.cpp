#include "ClientApplication.h"

using namespace qk::media;



std::vector<Vertex> g_Vertices = {
		{  0.0f,  0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f }
};


void Sandbox::init(int argc, char** argv) {
	init_subsystems();


	// Load shit from disk
	auto j = loadJSON("assets/pipeline.jsonc");
	auto layouts = VertexLayoutsFromJSON(j["layouts"]);
	auto materials = qk::media::MaterialsFromJSON(j["materials"]).value();
	auto sets = qk::media::DescriptorSetsFromJSON(j["descriptor_sets"]).value();


	// load layouts
	if (!layouts) {
		spdlog::critical("{}", layouts.error());
		std::abort();
	}


	// update the camera UBO
	g_Camera.view = glm::translate(g_Camera.view, { 0,0.5,0 });

	// upload vertex data
	rhi::RVertexBuffer vbo = rhi::VertexBuffer_ctor("vbo", rhi::StaticDraw);
	rhi::VertexBuffer_upload(vbo, g_Vertices);

	
	// create the uniform buffer that's linked by the descriptor set.
	rhi::RUniformBuffer ubo = rhi::UniformBuffer_ctor("CameraData");



	rhi::UniformBuffer_upload(ubo, &g_Camera, 1, sizeof(g_Camera));

	materials["mat_solid"].descriptor_set = rhi::DescriptorSet_ctor(sets["CameraDataDescriptorSet"]);

	rhi::HMaterial material = rhi::Material_ctor(
		materials["mat_solid"]
	);

	rhi::HMesh mesh = rhi::Mesh_ctor(rhi::Mesh_createinfo{
		.hVertexBuffers = {vbo},
		.hIndexBuffer = rhi::RIndexBuffer(rhi::NULL_RESOURCE),
		.layout_info = layouts.value()["Vertex"],
		.hMaterial = material
		});


	auto vao = rhi::Mesh_getVertexArray(mesh);



	rhi::CommandBuffer_createinfo CmdBufDesc;
	CmdBufDesc
		.preexecute(rhi::ClearCommand_desc{ 0.0,0.0,0.0,0.0, rhi::Color })
		.push(rhi::DrawArrays_desc{ material, vao, rhi::Triangle, 0, 3 });


	CmdBuf = rhi::CommandBuffer_ctor(CmdBufDesc);

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