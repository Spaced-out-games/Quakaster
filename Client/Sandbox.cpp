// -------------------------------------------------- includes -------------------------------------------------- //
#include "Sandbox.h"
#include <gfx/gfx.h>
#include <spdlog/spdlog.h>
#include <DrawCommand.h> // temp
//#include <glm/ext/matrix_transform.hpp>
//#include <gfx/InstancePool.h>
#include <utility/time.h>
#include "debugging/trace.h"
#include <net-v2/PeerRegistry.h>


// -------------------------------------------------- defines -------------------------------------------------- //
//#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/Screenshots/Screenshot (53).png"
#define TEST_IMAGE_PATH "C:/Users/devin/Desktop/burger-7419420_1280.jpg"


#define TEST_PIPELINE_PATH "assets/pipeline.jsonc"

// aliases
using namespace qk::media;
using namespace qk::gfx;
#include "kernel/UURH.h"

#include "utility/ScopeTimer.h"
#


// -------------------------------------------------- globals -------------------------------------------------- //

std::vector<glm::vec3> g_Vertices = {
	{-1.0f,  1.0f, 0.0f}, // top-left
	{-1.0f, -1.0f, 0.0f}, // bottom-left
	{ 1.0f,  1.0f, 0.0f}, // top-right
	{ 1.0f, -1.0f, 0.0f}, // bottom-right
};


std::vector<glm::mat4> g_Instances = {
    //glm::mat4(1.0),
    //glm::translate(glm::mat4(1.0), {0.2,0.0,0.0}),
    //glm::translate(glm::mat4(1.0), {0.2,0.2,0.0})
};




glm::vec3 tint = { 0.0, 0.0, 0.0 };

entt::registry reg;


//glm::mat4 m = { 1.0f };

// -------------------------------------------------- helpers -------------------------------------------------- //

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

void print_mat4(const glm::mat4& mat) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            std::printf("%8.3f ", mat[col][row]);
        }
        std::printf("\n");
    }
}

#include <random>
#include <glm/gtc/matrix_transform.hpp>

static std::mt19937 g_Rng{ 1337 }; // fixed seed so a crash replays

static glm::mat4 random_mat4() {
    std::uniform_real_distribution<float> pos(-1.0f, 1.0f), ang(0.0f, 0.0f), scl(0.01f, 0.01f);
    glm::mat4 m = glm::translate(glm::mat4(1.0f), { pos(g_Rng), pos(g_Rng), 0.0f });
    m = glm::rotate(m, ang(g_Rng), glm::vec3(0, 0, 1));
    return glm::scale(m, glm::vec3(scl(g_Rng)));
}



void Sandbox::init(int argc, char** argv) {

    

    spdlog::info(qk::trace("hello"));
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
    //VertexBuffer_upload(hInstanceBuffer, g_Instances);



    

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
    m_MeshID = renderer_test(r,
        "PBRPipeline"_sh,
        "VertexBuffer"_sh,
        "InstanceBuffer"_sh,
        "albedo"_sh,
        "tint"_sh,
        hInstanceBuffer
    );


    printf("// ---------------------------------- END GFX RSRC INIT ---------------------------------- //\n");


    printf("// ---------------------------------- LIST PARTITIONS BEFORE ---------------------------------- //\n");


    for (const auto& entry : r.LODData(m_MeshID)) {
        spdlog::info("Partition({}, {})", entry.offset, entry.size);
    }

    // preview for debugging
    //auto* pInstancePool = r.instancepool<glm::mat4>(mesh_id);

    instance = r.insert(m_MeshID, 0, glm::mat4{ 1.0f });





    //r.syncPartitionCache(mesh_id);



    printf("// ---------------------------------- LIST PARTITIONS AFTER ---------------------------------- //\n");


    for (const auto& entry : r.LODData(m_MeshID)) {
        spdlog::info("Partition({}, {})", entry.offset, entry.size);
    }
    
    r.syncPartitionCache(m_MeshID);
    
    auto recipe = r.compile();

    printf("// ---------------------------------- LIST COMMANDBUFFER ---------------------------------- //\n");

    spdlog::info("recipe.size(): {}", recipe.size());

    for (const auto& cmd : recipe.commands()) {
        std::cout << cmd << '\n';
    }


    for (int n = 10000; n > 0; --n) {
        m_Live.push_back(r.insert(m_MeshID, 0, random_mat4()));
    }
    r.flush_mesh(m_MeshID);


   

    // Command buffer
    //CommandBuffer_recipe cmdBuf;
    //cmdBuf
    //    .bindPipeline(hPipeline)
    //    .bindDescriptorSet(albedo, 0)
    //    .bindDescriptorSet(hTint, 1)
    //    .bindVertexBuffer(hInstanceBuffer, 1, 0, sizeof(Mat4))
    //    .bindVertexBuffer(hVertexBuffer, 0, 0, sizeof(Vertex))
    //    .drawArraysInstanced(0, 4, 3);

    
    if (r.needs_recompiled()) {
        r.syncPartitionCache(m_MeshID);
        auto recipe = r.compile();
        hCmdBuf = CommandBuffer_ctor(recipe);
        if (r.get_resource<HCommandBuffer>(0))
            r.remove_resource<HCommandBuffer>(0);
        r.define_resource(0, hCmdBuf);
        r.clear_flags();
    }
    

    //m_Live.push_back(r.insert(m_MeshID, 0, random_mat4()));
    //r.flush_mesh(m_MeshID);

}
 


void Sandbox::run() {
    using utility::ScopeTimer;
    static bool ran = false;
    static uint64_t last_report = qk::utility::now();
    static uint64_t frames = 0;

    {
        
        ScopeTimer t_frame("frame", m_Profilers.find("frame"));

        {
            ScopeTimer t("events", m_Profilers.find("events"));
            m_Window.pollEvents();
            m_Layers.propagate_events();
        }

        {
            ScopeTimer t("clear", m_Profilers.find("clear"));
            rhi::gl_clear();
        }

        if (!m_Live.empty()) {
            ScopeTimer t("execute", m_Profilers.find("execute"));
            CommandBuffer_execute(hCmdBuf);
        }

        {
            ScopeTimer t("layers", m_Profilers.find("layers"));
            m_Layers.render();
        }

        {
            ScopeTimer t("swap", m_Profilers.find("swap"));
            m_Window.swapBuffers();
        }

        setStatus(m_Window.shouldClose());
    }

    ++frames;
    const uint64_t now = qk::utility::now();
    if (now - last_report >= 1'000'000'000ULL) {
        const double elapsed = double(now - last_report) / 1e9;
        spdlog::warn("FPS: {}, instance count: {}", double(frames) / elapsed, m_Live.size());
        frames = 0;
        last_report = now;
    }
    m_Profilers.end_frame();
}

void Sandbox::shutdown() {

	r.destroy_all_resources();

}


std::unique_ptr<qk::Application> qk::create_application(int argc, char** argv) {
	return std::make_unique<Sandbox>();
}