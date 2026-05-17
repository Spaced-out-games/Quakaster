#pragma once
#include <Resource.h>
#include <vector>
#include <fnv1a.h>
#include <variant>

using namespace qk;


namespace qk::gfx::rhi {

    

    


}

/*
General idea:

    struct ResourceHandle {
        enum type;
        hash id;
    }

	virtual-Polymorphic base RenderPass type. 
	struct RenderPass
	{
		virtual void execute(RendererContext&) = 0;
		virtual ~RenderPass() = default;
	};

    struct RendererContext
{
    Registry* registry;
    CommandQueue* queue;
};

    class GeometryPass : public RenderPass
{
public:
    FramebufferHandle framebuffer;
    State state;
    std::vector<DrawCommand> draws;
    std::vector<ResourceHandle> deps;

    void execute(RendererContext& ctx) override
    {
        bindFramebuffer(framebuffer);
        applyState(state);

        for (auto& cmd : draws)
        {
            auto* mesh = ctx.registry->get(cmd.mesh);
            auto* mat  = ctx.registry->get(cmd.material);

            if (!mesh || !mat)
                continue;

            mat->bind();
            mat->update();
            mesh->bind();

            glDrawElementsInstanced(...);
        }
    }
};

 Maybe make polymorphic
struct DrawCommand
{
    MeshHandle mesh;
    MaterialHandle material;

    glm::mat4 transform;

    uint32_t instanceCount = 1;
};

Renderer holds unique<RenderPass> and draw() calls the whole thing. 


class Registry
{
public:
// consider a pattern like this. Will need type erasure since it's runtime.
    template<typename T>
    T* get(ResourceHandle h);

    void destroy(ResourceHandle h); // deferred internally
};
*/