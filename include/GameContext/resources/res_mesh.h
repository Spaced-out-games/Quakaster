#pragma once
#include <GL/glew.h>
#include <include/GameContext/graphics/VAO.h>
#include <include/GameContext/graphics/VBO.h>
#include <include/GameContext/graphics/EBO.h>
#include <include/thirdparty/entt.hpp>

/*
ALERT!!!

This is not a real type. It only acts as a fake container for initializing mesh components in a way that feels like OOP
*/
struct Mesh {
    // Mesh constructor that creates and binds VAO, VBO, and EBO
    template <typename vertex_t>
    Mesh(entt::handle& entity, const std::vector<vertex_t>& vertices, const std::vector<uint32_t>& indices, std::string shader_name, std::string vertex_path, std::string fragment_path) {
        // Create and bind VAO
        entity.emplace<VAO>();
        auto& vao = entity.get<VAO>();
        vao.bind();

        // Create and bind VBO
        entity.emplace<VBO>(vertices);
        auto& vbo = entity.get<VBO>();
        vbo.bind();

        // Create and bind EBO
        entity.emplace<EBO>();
        auto& ebo = entity.get<EBO>();
        ebo.init(indices); // Upload indices to the EBO

        // Store the shader in the entity
        entity.emplace<Shader>(shader_name, vertex_path, fragment_path);

        // Set vertex attribute pointers using the shader program
        vertex_t::set_pointers();

        // Unbind VBO, EBO, and VAO
        VBO::unbind();
        EBO::unbind();
        VAO::unbind();
    }

    ~Mesh() = default;

    // Draw all meshes in the registry
    static void draw_all(entt::registry& registry) {
        // Iterate over entities with VAO, VBO, EBO, and Shader components
        auto view = registry.view<VAO, VBO, EBO, Shader>();
        for (auto entity : view) {
            auto& vao = view.get<VAO>(entity);
            auto& vbo = view.get<VBO>(entity);
            auto& ebo = view.get<EBO>(entity);
            auto& shader = view.get<Shader>(entity);

            if (registry.all_of<Texture>(entity))
            {
                registry.get<Texture>(entity).bind();
            }

            //if (registry.all_of<Transform>(entity)) {

            // Bind the shader for this mesh
            shader->bind();
            vao.bind();
            ebo.bind(); // Bind the EBO before drawing
            glDrawElements(GL_TRIANGLES, ebo.get_index_count(), GL_UNSIGNED_INT, 0); // Draw using indices
            VAO::unbind();
            //VBO::unbind();
            
            EBO::unbind(); // Unbind the EBO after drawing
        }
    }
};
