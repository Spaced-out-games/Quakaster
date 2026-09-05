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



void SparseSetTest() {
}

void PartitionTest() {
    spdlog::info("=== InstancePool / PartitionTest ===");

    InstancePool pool;
    assert(pool.m_Storage == nullptr);

    spdlog::info("Initial pool created");

    // Add another partition.
    pool.add_partition();
    spdlog::info("Added partition 1");

    std::vector<int> buf;

    // ------------------------------------------------------------
    // Partition 0
    // ------------------------------------------------------------

    spdlog::info("[P0] Inserting 420, 69, 2004");

    auto a = pool.insert(420, 0);
    auto b = pool.insert(69, 0);
    auto c = pool.insert(2004, 0);

    spdlog::info(
        "[P0] Handles: a=({}, {}), b=({}, {}), c=({}, {})",
        a.sparse, a.generation,
        b.sparse, b.generation,
        c.sparse, c.generation
    );

    pool.flush(buf);

    spdlog::info(
        "[P0] After insertion: [{}, {}, {}]",
        buf[0], buf[1], buf[2]
    );

    assert(buf.size() == 5);
    assert(buf[0] == 420);
    assert(buf[1] == 69);
    assert(buf[2] == 2004);

    // Update
    spdlog::info("[P0] Updating a -> 99");

    pool.set(a, 99);
    pool.flush(buf);

    spdlog::info(
        "[P0] After update: [{}, {}, {}]",
        buf[0], buf[1], buf[2]
    );

    assert(buf[0] == 99);
    assert(buf[1] == 69);
    assert(buf[2] == 2004);

    // ------------------------------------------------------------
    // Delete beginning
    // ------------------------------------------------------------

    spdlog::info("[P0] Removing beginning element a");

    pool.remove(a);

    assert(!pool.valid(a));
    assert(pool.valid(b));
    assert(pool.valid(c));

    pool.flush(buf);

    spdlog::info(
        "[P0] After removing a: [{}, {}, {}]",
        buf[0], buf[1], buf[2]
    );

    // C moved into A's old slot.
    assert(buf[0] == 2004);
    assert(buf[1] == 69);
    assert(buf[2] == -1);

    // ------------------------------------------------------------
    // Re-use insertion
    // ------------------------------------------------------------

    spdlog::info("[P0] Reusing removed handle slot with 1234");

    auto d = pool.insert(1234, 0);

    spdlog::info(
        "[P0] d=({}, {}), old a=({}, {})",
        d.sparse, d.generation,
        a.sparse, a.generation
    );

    assert(d.sparse == a.sparse);
    assert(pool.valid(d));

    pool.flush(buf);

    spdlog::info(
        "[P0] After reuse: [{}, {}, {}]",
        buf[0], buf[1], buf[2]
    );

    assert(buf[0] == 2004);
    assert(buf[1] == 69);
    assert(buf[2] == 1234);

    // ------------------------------------------------------------
    // Delete end
    // ------------------------------------------------------------

    spdlog::info("[P0] Removing end element d");

    pool.remove(d);

    assert(!pool.valid(d));
    assert(pool.valid(b));
    assert(pool.valid(c));

    pool.flush(buf);

    spdlog::info(
        "[P0] After removing d: [{}, {}, {}]",
        buf[0], buf[1], buf[2]
    );

    assert(buf[0] == 2004);
    assert(buf[1] == 69);
    assert(buf[2] == -1);

    // ------------------------------------------------------------
    // Delete beginning again
    // ------------------------------------------------------------

    spdlog::info("[P0] Removing beginning element c");

    assert(pool.m_HandleTable.try_get(c)->local_sparse == 0);

    pool.remove(c);

    assert(!pool.valid(c));
    assert(pool.valid(b));

    pool.flush(buf);

    spdlog::info(
        "[P0] After removing c: [{}, {}, {}]",
        buf[0], buf[1], buf[2]
    );

    assert(buf[0] == 69);
    assert(buf[1] == -1);
    assert(buf[2] == -1);

    // ------------------------------------------------------------
    // Partition 1
    // ------------------------------------------------------------

    spdlog::info("[P1] Inserting 111, 222, 333");

    auto e = pool.insert(111, 1);
    auto f = pool.insert(222, 1);
    auto g = pool.insert(333, 1);

    pool.flush(buf);

    spdlog::info(
        "[P1] After insertion: [{}, {}, {}]",
        buf[4], buf[5], buf[6]
    );

    assert(buf.size() == 8);

    assert(buf[0] == 69);
    assert(buf[1] == -1);
    assert(buf[2] == -1);

    assert(buf[4] == 111);
    assert(buf[5] == 222);
    assert(buf[6] == 333);

    assert(pool.valid(b));
    assert(pool.valid(e));
    assert(pool.valid(f));
    assert(pool.valid(g));

    // ------------------------------------------------------------
    // Delete beginning of partition 1
    // ------------------------------------------------------------

    spdlog::info("[P1] Removing beginning element e");

    pool.remove(e);

    assert(!pool.valid(e));
    assert(pool.valid(f));
    assert(pool.valid(g));

    pool.flush(buf);

    spdlog::info(
        "[P1] After removing e: [{}, {}, {}]",
        buf[4], buf[5], buf[6]
    );

    assert(buf[4] == 333);
    assert(buf[5] == 222);
    assert(buf[6] == -1);

    // ------------------------------------------------------------
    // Delete end of partition 1
    // ------------------------------------------------------------

    spdlog::info("[P1] Removing g");

    pool.remove(g);

    assert(!pool.valid(g));
    assert(pool.valid(f));

    pool.flush(buf);

    spdlog::info(
        "[P1] After removing g: [{}, {}, {}]",
        buf[4], buf[5], buf[6]
    );

    assert(buf[4] == 222);
    assert(buf[5] == -1);
    assert(buf[6] == -1);

    // ------------------------------------------------------------
    // Delete final element
    // ------------------------------------------------------------

    spdlog::info("[P1] Removing final element f");

    pool.remove(f);

    assert(!pool.valid(f));
    assert(pool.valid(b));

    pool.flush(buf);

    spdlog::info(
        "[P1] After emptying partition: [{}, {}, {}]",
        buf[4], buf[5], buf[6]
    );

    assert(buf[0] == 69);
    assert(buf[4] == -1);
    assert(buf[5] == -1);
    assert(buf[6] == -1);

    // ------------------------------------------------------------
    // Insert into emptied partition
    // ------------------------------------------------------------

    spdlog::info("[P1] Inserting 777 into empty partition");

    auto h = pool.insert(777, 1);

    assert(pool.valid(h));

    pool.flush(buf);

    spdlog::info(
        "[P1] After reuse of empty partition: [{}, {}, {}]",
        buf[4], buf[5], buf[6]
    );

    assert(buf[4] == 777);
    assert(buf[5] == -1);
    assert(buf[6] == -1);

    // ------------------------------------------------------------
    // Handle reuse
    // ------------------------------------------------------------

    spdlog::info("[P1] Removing h and testing generation reuse");

    pool.remove(h);

    assert(!pool.valid(h));

    auto i = pool.insert(888, 1);

    spdlog::info(
        "[P1] Old handle h=({}, {}), new handle i=({}, {})",
        h.sparse, h.generation,
        i.sparse, i.generation
    );

    assert(i.sparse == h.sparse);
    assert(i.generation != h.generation);
    assert(pool.valid(i));
    assert(!pool.valid(h));

    pool.flush(buf);

    spdlog::info(
        "[P1] After generation reuse: [{}, {}, {}]",
        buf[4], buf[5], buf[6]
    );

    assert(buf[4] == 888);

    // ------------------------------------------------------------
    // Final co-occupancy / stale-storage checks
    // ------------------------------------------------------------

    spdlog::info("[FINAL] Checking stale storage invariants");

    assert(buf[1] == -1);
    assert(buf[2] == -1);

    assert(buf[5] == -1);
    assert(buf[6] == -1);

    spdlog::info("=== PartitionTest PASSED ===");
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