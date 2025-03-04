#pragma once
#include <GL/glew.h>
#include <include/thirdparty/glm/glm.hpp>
#include <string>
#include <unordered_map>
#define STB_IMAGE_IMPLEMENTATION
#include <include/thirdparty/stb_image.h>
#include <include/resources/res_shader.h>

extern std::string root_directory;




struct Texture
{
	Texture(std::string path, GLenum texture_type = GL_TEXTURE_2D): texture_type(texture_type)
	{
		glGenTextures(1, &tex_ID);
		glBindTexture(texture_type, tex_ID);
		
		// Wrapping and filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); // Lowest level
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);  // Highest level
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);


		path = root_directory + path;

		// load into RAM
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channel_count, 0);

		if (data)
		{
			// get the right format
			GLenum format = (channel_count == 4) ? GL_RGBA : GL_RGB;

			// Pass data to OpenGL
			glTexImage2D(texture_type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			// std::cout << "Failed to load texture at location: " << path << '\n';
			__debugbreak();


		}

		stbi_image_free(data);

	}
	void bind()
	{
		glBindTexture(texture_type, tex_ID);
	}

	static void unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLuint tex_ID;
	GLenum texture_type;
	int width;
	int height;
	int channel_count;
};