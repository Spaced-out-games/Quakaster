#pragma once
#include <GL/glew.h>
#include <include/thirdparty/glm/glm.hpp>
#include <string>
#include <unordered_map>
#define STB_IMAGE_IMPLEMENTATION
#include <include/thirdparty/stb_image.h>
#include <include/resources/res_shader.h>





struct Texture
{
	Texture() = default;
	Texture(std::string path, GLenum texture_type = GL_TEXTURE_2D) { init(path, texture_type); }
	void init(std::string path, GLenum texture_type = GL_TEXTURE_2D)
	{
		mTextype = texture_type;
		glGenTextures(1, &mTexID);
		glBindTexture(texture_type, mTexID);
		
		// Wrapping and filtering. Needs to be more flexible and actually use convars
		glTexParameteri(mTextype, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(mTextype, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(mTextype, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(mTextype, GL_TEXTURE_BASE_LEVEL, 0); // Lowest level
		glTexParameteri(mTextype, GL_TEXTURE_MAX_LEVEL, 4);  // Highest level
		glTexParameteri(mTextype, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(mTextype, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(mTextype);


		path = std::string(ROOT_DIRECTORY) + path;

		// load into RAM
		unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &mChannelCount, 0);

		if (data)
		{
			// get the right format
			GLenum format = (mChannelCount == 4) ? GL_RGBA : GL_RGB;

			// Pass data to OpenGL
			glTexImage2D(mTextype, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, data);
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
		glBindTexture(mTextype, mTexID);
	}

	static void unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	private:
		GLuint mTexID = 0;
		GLenum mTextype = GL_TEXTURE_2D;
		int mWidth = 0;
		int mHeight = 0;
		int mChannelCount = 3;
};