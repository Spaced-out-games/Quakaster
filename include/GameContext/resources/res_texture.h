#pragma once
#include <GL/glew.h>
#include <include/thirdparty/glm/glm.hpp>
#include <string>
#include <unordered_map>
#define STB_IMAGE_IMPLEMENTATION
#include <include/thirdparty/stb_image.h>
#include <include/GameContext/resources/res_shader.h>

extern std::string root_directory;



struct TextureAtlasRegion
{
	glm::vec<2, uint16_t> start;
	glm::vec<2, uint16_t> end;

	TextureAtlasRegion(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
		: start(start_x, start_y), end(end_x, end_y) {}

	inline glm::vec<2, uint16_t> size() const { return end - start; }
};


struct Texture
{
	Texture(std::string path, GLenum texture_type = GL_TEXTURE_2D, GLenum texture_unit = GL_TEXTURE0) : unit(texture_unit)
	{	
		constexpr int desired_channel_count = 4;
		// generate buffers
		glGenTextures(1, &ID);
		glActiveTexture(texture_unit);
		glBindTexture(texture_type, ID);

		handle hdl = load_image(path, desired_channel_count);

		if (hdl.data)
		{
			GLenum format = (hdl.channel_count == 4) ? GL_RGBA : GL_RGB;
			glTexImage2D(texture_type, 0, format, hdl.width, hdl.height, 0, format, GL_UNSIGNED_BYTE, hdl.data);
			//glGenerateMipmap(texture_type);

			// Set texture parameters
			glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

//		glBindTexture(texture_type, 0)
	}

	~Texture()
	{
		glDeleteTextures(1, &ID);
	}

	void bind() const
	{
		glActiveTexture(unit);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void set_uniforms(Shader& shader)
	{
		shader->operator[]("texture_" + std::to_string(unit - GL_TEXTURE0  ) ) = 0;
	}

	void unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLuint get_ID() { return ID; }

	

	class handle
	{
		stbi_uc* data; // unsigned char*
		int width;
		int height;
		int channel_count;
		friend Texture;
		public:
			inline const int get_width() const { return width; }
			inline const int get_height() const { return height; }
			inline const void* get_data_ptr() const { return data; }
			~handle() {
				stbi_image_free(data);
			}
	};

	

	static handle load_image(std::string& path, int desired_channel_count)
	{
		handle hdl;
		std::string complete_path = root_directory + path;
		hdl.data = stbi_load(complete_path.c_str(), &hdl.width, &hdl.height, &hdl.channel_count, desired_channel_count);
		if (!hdl.data)
		{
			throw std::runtime_error("Failed to load texture at path: " + complete_path);
		}
		return hdl;
	}


	private:
		GLuint ID;
		GLenum unit;
		// Handle to a loaded image. Read-only members

};
struct TextureAtlas
{
	// Size must be a power of two
	TextureAtlas(uint16_t size)
	{
			
	}

	std::unordered_map<uint16_t, TextureAtlasRegion> regions;


};

