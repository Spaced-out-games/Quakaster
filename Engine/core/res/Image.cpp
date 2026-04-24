/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/res/Image.h
/// Purpose: Implements a structure that can load images from disk
/// ************************************************************************************************** 

#include "Image.h"
#include "stb_image.h"
#include <cassert>
namespace qk::resource {

	Image::Image(const std::string& path) {
		load_image(*this, path);
	}

	Image::Image(int width, int height, int channels) {
		make_image(*this, width, height, channels);
	}

	void Image::reload(const std::string& path) {
		destroy();
		load_image(*this, path);
	}

	void Image::clear() {
		make_image(*this, m_Width, m_Height, m_Channels);
	}


	Image::~Image() {
		destroy();
	}

	Image::Image(Image&& other) noexcept
		: m_Data(other.m_Data),
		m_Width(other.m_Width),
		m_Height(other.m_Height),
		m_Channels(other.m_Channels)
	{
		other.m_Data = nullptr;
		other.m_Width = 0;
		other.m_Height = 0;
		other.m_Channels = 0;
	}

	Image& Image::operator=(Image&& other) noexcept {
		if (this != &other) {
			if (m_Data)
				stbi_image_free(m_Data);

			m_Data = other.m_Data;
			m_Width = other.m_Width;
			m_Height = other.m_Height;
			m_Channels = other.m_Channels;

			other.m_Data = nullptr;
		}
		return *this;
	}

	uint8_t* Image::data() const { return m_Data; }
	int Image::width() const { return m_Width; }
	int Image::height() const { return m_Height; }
	int Image::channels() const { return m_Channels; }
	size_t Image::size_bytes() const {
		return (size_t)m_Width * (size_t)m_Height * (size_t)m_Channels;
	}

	void Image::destroy() {

		stbi_image_free(m_Data);
		m_Data = nullptr;
		m_Channels = 0;
		m_Height = 0;
		m_Width = 0;
	}

	void load_image(Image& target, const std::string& path)
	{
		assert(path != "");
		assert(target.m_Data == nullptr);



		target.m_Data = stbi_load(
			path.c_str(),
			&target.m_Width,
			&target.m_Height,
			&target.m_Channels,
			0
		);

		if (!target.m_Data)
			throw std::runtime_error(stbi_failure_reason());
	}

	void make_image(Image& target, int width, int height, int channels)
	{

		assert(target.m_Data == nullptr);
		
		assert(channels > 0);
		assert(height > 0);
		assert(width > 0);

		size_t size = (size_t)width * (size_t)height * (size_t)channels;

		target.m_Data = (uint8_t*)malloc(size);
		if (!target.m_Data)
			throw std::runtime_error("malloc failed");

		memset(target.m_Data, 0, size);

		target.m_Width = width;
		target.m_Height = height;
		target.m_Channels = channels;
	}

}