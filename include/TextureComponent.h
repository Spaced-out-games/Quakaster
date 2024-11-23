#pragma once
#include <string>
#include <unordered_map>
#include <GL/glew.h> // Ensure you're using the correct OpenGL headers
#include <stb_image.h>
#include <iostream> // For error logging

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


class TexturePool
{
private:
    std::unordered_map<std::string, GLuint> IDs; // Stores texture IDs

public:
    GLuint load_texture(const std::string& path) // Pass by const reference
    {
        // Check if the texture is already loaded
        auto it = IDs.find(path);
        if (it != IDs.end()) {
            return it->second; // Return existing texture ID
        }

        // Variables to hold texture information
        GLuint ID;
        int width, height, channel_count;

        // Load the image using stb_image
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channel_count, 0);
        if (data == nullptr) {
            std::cerr << "Failed to load texture: " << path << std::endl;
            return 0; // Return 0 to indicate failure
        }

        // Generate and bind a new OpenGL texture
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Upload the texture data to OpenGL
        if (channel_count == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (channel_count == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else {
            std::cerr << "Unsupported texture format in: " << path << std::endl;
            stbi_image_free(data);
            glBindTexture(GL_TEXTURE_2D, 0);
            return 0; // Return 0 to indicate failure
        }

        // Generate mipmaps (optional)
        glGenerateMipmap(GL_TEXTURE_2D);

        // Free image data after uploading to GPU
        stbi_image_free(data);

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // Store the texture ID and return it
        IDs.emplace(path, ID);
        return ID;
    }
};

// might be good to separate these in order to not couple their includes

struct TextureComponent
{
    GLuint ID;

    TextureComponent() : ID(0) {}

    void load(const std::string& path, TexturePool& pool) // Pass TexturePool by reference
    {
        ID = pool.load_texture(path);
    }
};
