#pragma once
#include <GL/glew.h>
#include <include/GameContext/graphics/VAO.h>
#include <include/GameContext/graphics/VBO.h> // Just in case
#include <include/thirdparty/entt.hpp>
/*
ALERT!!!

This is not a real type. It only acts as a fake container for initializing mesh components in a way that feels like OOP
*/
struct Mesh {
    // Mesh constructor that creates and binds VAO and VBO
    template <typename vertex_t>
    Mesh(entt::handle& entity, const std::vector<vertex_t>& vertices, Shader shader) {
        // Create and bind VAO
        entity.emplace<VAO>();
        auto& vao = entity.get<VAO>();
        vao.bind();

        // Create and bind VBO
        entity.emplace<VBO>(vertices);
        auto& vbo = entity.get<VBO>();
        vbo.bind();

        // Store the shader in the entity
        entity.emplace<Shader>(shader);

        // Set vertex attribute pointers using the shader program
        vertex_t::set_pointers();

        // Unbind VBO and VAO
        VBO::unbind();
        vao.unbind();
    }
    ~Mesh() = default;

    // Draw all meshes in the registry
    static void draw_all(entt::registry& registry) {
        // Iterate over entities with VAO, VBO, and Shader components
        auto view = registry.view<VAO, VBO, Shader>();
        for (auto entity : view) {
            auto& vao = view.get<VAO>(entity);
            auto& vbo = view.get<VBO>(entity);
            auto& shader = view.get<Shader>(entity);

            if (registry.all_of<Transform>(entity))
            {
                auto& transform = registry.get<Transform>(entity);
                shader->operator[]("model") = transform.get_matrix();
            }


            // Bind the shader for this mesh
            shader->bind();
            vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, vbo.get_vertex_count());
            VAO::unbind();
        }
    }
};
