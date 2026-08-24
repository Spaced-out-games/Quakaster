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



void SparseSetTest() {
    HandleTable<i32> map;

    auto first = map.insert(1);
    auto second = map.insert(2);
    auto third = map.insert(3);

    i32* pFirst = map.try_get(first);
    i32* pSecond = map.try_get(second);
    i32* pThird = map.try_get(third);

    assert(map.valid(first));
    assert(map.valid(second));
    assert(map.valid(third));

    assert(pFirst && *pFirst == 1);
    assert(pSecond && *pSecond == 2);
    assert(pThird && *pThird == 3);

    map.remove(first);
    assert(!map.valid(first));
    auto fourth = map.insert(3);

    assert(map.valid(fourth));
    assert(first.sparse == fourth.sparse && first.generation == fourth.generation - 1);

    map.set(fourth, 69);
    i32* pFourth = map.try_get(fourth);
    assert(pFourth && *pFourth == 69);




    spdlog::info("SparseSetTest PASSED!");
}

void PartitionTest() {
    Partition<i32> p;

    // general case insertion test
    i32 first = p.insert(1);
    i32 second = p.insert(2);
    i32 third = p.insert(3);
    i32 fourth = p.insert(4);

    i32 back = p.remove(second);

    
   
    
    std::vector<i32> testBuffer;
    testBuffer.resize(4);
    p.flush(testBuffer.data());

    assert(testBuffer[0] == 1);
    assert(testBuffer[1] == 4);
    assert(testBuffer[2] == 3);
    assert(testBuffer[3] == -1u);
    assert(p.size() == 3);
    
}



entt::registry reg;
void Sandbox::init(int argc, char** argv) {
    SparseSetTest();
    PartitionTest();
    init_subsystems();
    loader_demo(reg, entt::null);

    // Define assets
    const auto pipeline_json = loadJSON(TEST_PIPELINE_PATH);
    loadPipelinesFromJSON(pipeline_json, pipelines);
    loadDescriptorSetsFromJSON(pipeline_json, sets);
    registry_upload(DescriptorSets, sets);
    registry_upload(Pipelines, pipelines);


    // retreive the handles
    hPipeline = Pipelines.peek("PBRPipeline"_sh);
    hGlobals = DescriptorSets.peek("hGlobals"_sh);
    hMaterial = DescriptorSets.peek("hMaterial"_sh);


    // build the vertex buffer
    hVertexBuffer = VertexBuffer_ctor(BufferUsage::StaticDraw);
    GBuffers.insert("VertexBuffer"_sh, hVertexBuffer);
    VertexBuffer_upload(hVertexBuffer, g_Vertices);

    // build the instancebuffer
    hInstanceBuffer = VertexBuffer_ctor(BufferUsage::StaticDraw);
    GBuffers.insert("InstanceBuffer"_sh, hInstanceBuffer);
    VertexBuffer_upload(hInstanceBuffer, g_Instances);



    hTex = load_or_reload(Textures, TEST_IMAGE_PATH);



    // Texture

    DescriptorSet_setResource(hGlobals, 0, Textures.peek(qk::string_hash{ TEST_IMAGE_PATH }));

    // Uniform buffer
    hUniformBuffer = UniformBuffer_ctor();
    GBuffers.insert("hUniformBuffer"_sh, hUniformBuffer);
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
    CommandBuffers.insert(0, hCmdBuf);

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