#include "Sandbox.h"
#include <gfx/gfx.h>
using namespace qk::media;
using namespace qk::gfx;


#include "math/geometric/Mat4.h"







std::vector<Vertex> g_Vertices = {
	{-1.0f,  1.0f, 0.0f}, // top-left
	{-1.0f, -1.0f, 0.0f}, // bottom-left
	{ 1.0f,  1.0f, 0.0f}, // top-right
	{ 1.0f, -1.0f, 0.0f}, // bottom-right
};


std::vector<Mat4> g_Instances = {
    Mat4(1.0),
    Mat4(1.0).translate({0.2,0.0,0.0}),
    Mat4(1.0).translate({0.2,0.2,0.0})
};

glm::vec3 tint = { 0.5, 0.25, 0.0 };

#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (53).png"

#define TEST_PIPELINE_PATH "assets/pipeline.jsonc"

//#include <gfx/InstancePool.h>


#include "Handle.h"
#include <Partition.h>
#include "gfx/InstancePool.h"




entt::registry reg;
void Sandbox::init(int argc, char** argv) {
    init_subsystems();
    loader_demo(reg, entt::null);


    // Define assets
    const auto pipeline_json = loadJSON(TEST_PIPELINE_PATH);
    loadPipelinesFromJSON(pipeline_json, pipelines);
    loadDescriptorSetsFromJSON(pipeline_json, sets);
    
    // load descriptor sets into renderer
    for (const auto& [name, recipe] : sets) {
        HDescriptorSet hDescriptorSet = rhi::DescriptorSet_ctor(recipe);
        r.define_resource(qk::string_hash{ name }, hDescriptorSet);
    }

    for (const auto& [name, recipe] : pipelines) {
        HPipeline hPipeline = rhi::Pipeline_ctor(recipe);
        r.define_resource(qk::string_hash{ name }, hPipeline);
    }
    


    // build the vertex buffer
    hVertexBuffer = VertexBuffer_ctor(BufferUsage::StaticDraw);
    r.define_resource("VertexBuffer"_sh, hVertexBuffer);
    VertexBuffer_upload(hVertexBuffer, g_Vertices);

    // build the instancebuffer
    hInstanceBuffer = VertexBuffer_ctor(BufferUsage::StaticDraw);
    r.define_resource("InstanceBuffer"_sh, hInstanceBuffer);
    VertexBuffer_upload(hInstanceBuffer, g_Instances);
    


    // retreive the handles

    hPipeline = r.get_resource<HPipeline>("PBRPipeline"_sh);
    hGlobals = r.get_resource<HDescriptorSet>("hGlobals"_sh);
    hMaterial = r.get_resource<HDescriptorSet>("hMaterial"_sh);








    hTex = r.load_or_reload_texture(TEST_IMAGE_PATH);
    


    // Texture


    DescriptorSet_setResource(hGlobals, 0, r.get_resource<HTexture>(qk::string_hash{ TEST_IMAGE_PATH }));



    // Uniform buffer
    hUniformBuffer = UniformBuffer_ctor();

    r.define_resource("hUniformBuffer"_sh, hUniformBuffer);
    UniformBuffer_upload(hUniformBuffer, &tint, 3 * sizeof(float));
    DescriptorSet_setResource(hMaterial, 0, hUniformBuffer);



    // Command buffer
    CommandBuffer_recipe cmdBuf;
    cmdBuf
        .bindPipeline(hPipeline)
        .bindDescriptorSet(hGlobals, 0)
        .bindDescriptorSet(hMaterial, 1)
        .bindVertexBuffer(hInstanceBuffer, 1, 0, sizeof(Mat4))
        .bindVertexBuffer(hVertexBuffer, 0, 0, sizeof(Vertex))
        .drawArraysInstanced(0, 4, 3);

    hCmdBuf = CommandBuffer_ctor(cmdBuf);
    r.define_resource(0, hCmdBuf);
//    CommandBuffers.insert(0, hCmdBuf);

    

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

	r.destroy_all_resources();

}


std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<Sandbox>();
}