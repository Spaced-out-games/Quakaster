#include "Sandbox.h"
#include <gfx/gfx.h>
using namespace qk::media;
using namespace qk::gfx;

#include <spdlog/spdlog.h>

#include <DrawCommand.h> // temp


#include <glm/ext/matrix_transform.hpp>



std::vector<glm::vec3> g_Vertices = {
	{-1.0f,  1.0f, 0.0f}, // top-left
	{-1.0f, -1.0f, 0.0f}, // bottom-left
	{ 1.0f,  1.0f, 0.0f}, // top-right
	{ 1.0f, -1.0f, 0.0f}, // bottom-right
};


std::vector<glm::mat4> g_Instances = {
    glm::mat4(1.0),
    //glm::translate(glm::mat4(1.0), {0.2,0.0,0.0}),
    //glm::translate(glm::mat4(1.0), {0.2,0.2,0.0})
};

glm::vec3 tint = { 0.5, 0.25, 0.0 };

#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (53).png"

#define TEST_PIPELINE_PATH "assets/pipeline.jsonc"

//#include <gfx/InstancePool.h>




template <typename ...Ts>
MeshDefinitionID renderer_test(Renderer<Ts...>& r,
    u64 pipeline_id,
    u64 VertexBuffer_id,
    u64 InstanceBuffer_id,
    u64 GlobalsDescriptorSetID,
    u64 MaterialDescriptorSetID,
    HVertexBuffer hInstanceBuffer) {
    // create a new mesh definition



     return r.new_definition()
         .pushPipeline(pipeline_id)
         .pushVertexBuffer(VertexBuffer_id, 0, sizeof(glm::vec3), 0)
         .pushVertexBuffer(InstanceBuffer_id, 0, sizeof(glm::mat4), 1)
         .pushDescriptorSet(GlobalsDescriptorSetID, 0)
         .pushDescriptorSet(MaterialDescriptorSetID, 1)
         .pushInstancePool<glm::mat4>(hInstanceBuffer)
         .setVertexBufferRange(0, 4)
         .setLODCount(1)
         .finalize();



}


entt::registry reg;
void Sandbox::init(int argc, char** argv) {

    printf("// ---------------------------------- INITIALIZATION ---------------------------------- //\n");


    init_subsystems();
    loader_demo(reg, entt::null);


    printf("// ---------------------------------- RESOURCE LOADING ---------------------------------- //\n");

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

    
    // get the pipeline
    hPipeline = r.get_resource<HPipeline>("PBRPipeline"_sh);

    // get the descriptor sets
    hTextureSet = r.get_resource<HDescriptorSet>("albedo"_sh);
    hTintSet = r.get_resource<HDescriptorSet>("tint"_sh);



    printf("// ---------------------------------- BUFFER BUILD ---------------------------------- //\n");


    hVertexBuffer = VertexBuffer_ctor(BufferUsage::StaticDraw);
    r.define_resource("VertexBuffer"_sh, hVertexBuffer);
    VertexBuffer_upload(hVertexBuffer, g_Vertices);

    hInstanceBuffer = VertexBuffer_ctor(BufferUsage::StaticDraw);
    r.define_resource("InstanceBuffer"_sh, hInstanceBuffer);
    VertexBuffer_upload(hInstanceBuffer, g_Instances);



    

    printf("// ---------------------------------- DESCRIPTOR LOAD ---------------------------------- //\n");


    // initialize descriptors
    hTextureResource = r.load_or_reload_texture(TEST_IMAGE_PATH);
    hTint = UniformBuffer_ctor();
    r.define_resource("hUniformBuffer"_sh, hTint);
    UniformBuffer_upload(hTint, &tint, 3 * sizeof(float));

    // add to descriptor set
    DescriptorSet_setResource(hTextureSet, 0, r.get_resource<HTexture>(qk::string_hash{ TEST_IMAGE_PATH }));
    DescriptorSet_setResource(hTintSet, 0, hTint);
    

    // define a mesh
    MeshDefinitionID mesh_id = renderer_test(r,
        "PBRPipeline"_sh,
        "VertexBuffer"_sh,
        "InstanceBuffer"_sh,
        "albedo"_sh,
        "tint"_sh,
        hInstanceBuffer
    );


    printf("// ---------------------------------- END GFX RSRC INIT ---------------------------------- //\n");


    printf("// ---------------------------------- LIST PARTITIONS BEFORE ---------------------------------- //\n");


    for (const auto& entry : r.LODData(mesh_id)) {
        spdlog::info("Partition({}, {})", entry.offset, entry.size);
    }

    // preview
    const auto* pInstancePool = r.instancepool<glm::mat4>(mesh_id);
    //auto handle = r.insert(mesh_id, 0, glm::mat4(1.0));
    //r.update(handle, (glm::mat4)glm::mat4_cast(q));

    //r.flush_all();
    //r.syncPartitionCache(mesh_id);



    printf("// ---------------------------------- LIST PARTITIONS AFTER ---------------------------------- //\n");


    for (const auto& entry : r.LODData(mesh_id)) {
        spdlog::info("Partition({}, {})", entry.offset, entry.size);
    }


    auto recipe = r.compile();

    printf("// ---------------------------------- LIST COMMANDBUFFER ---------------------------------- //\n");

    spdlog::info("recipe.size(): {}", recipe.size());

    for (const auto& cmd : recipe.commands()) {
        std::cout << cmd << '\n';
    }


    


   

    // Command buffer
    //CommandBuffer_recipe cmdBuf;
    //cmdBuf
    //    .bindPipeline(hPipeline)
    //    .bindDescriptorSet(albedo, 0)
    //    .bindDescriptorSet(hTint, 1)
    //    .bindVertexBuffer(hInstanceBuffer, 1, 0, sizeof(Mat4))
    //    .bindVertexBuffer(hVertexBuffer, 0, 0, sizeof(Vertex))
    //    .drawArraysInstanced(0, 4, 3);

    
    
    // upload commandbuffer
    hCmdBuf = CommandBuffer_ctor(recipe);
    r.define_resource(0, hCmdBuf);

    

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