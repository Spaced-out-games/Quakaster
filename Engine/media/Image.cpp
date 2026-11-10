/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/res/Image.h
/// Purpose: Implements a structure that can load images from disk
/// ************************************************************************************************** 

#include "Image.h"
#include "../vendor/stb_image.h"
#include <cassert>
#include <spdlog/spdlog.h>
namespace qk::media {

	Image::Image(const std::string& path) {
		load_image(*this, path);
	}

	Image::Image(u32 width, u32 height, u32 channels) {
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
		: m_pData(other.m_pData),
		m_Width(other.m_Width),
		m_Height(other.m_Height),
		m_Channels(other.m_Channels)
	{
		other.m_pData = nullptr;
		other.m_Width = 0;
		other.m_Height = 0;
		other.m_Channels = 0;
	}

	Image& Image::operator=(Image&& other) noexcept {
		if (this != &other) {
			if (m_pData)
				stbi_image_free(m_pData);

			m_pData = other.m_pData;
			m_Width = other.m_Width;
			m_Height = other.m_Height;
			m_Channels = other.m_Channels;

			other.m_pData = nullptr;
		}
		return *this;
	}

	uint8_t* Image::data() const { return m_pData; }
	u32 Image::width() const { return m_Width; }
	u32 Image::height() const { return m_Height; }
	u32 Image::channels() const { return m_Channels; }
	size_t Image::size_bytes() const {
		return (size_t)m_Width * (size_t)m_Height * (size_t)m_Channels;
	}

	void Image::destroy() {
		if (!m_pData) return;
		stbi_image_free(m_pData);
		m_pData = nullptr;
		m_Channels = 0;
		m_Height = 0;
		m_Width = 0;
	}

	bool load_image(Image& target, const std::string& path)
	{
		QK_ASSERT(!path.empty(), "path was empty");
		QK_ASSERT(target.m_pData == nullptr, "m_pData was nullptr");

		int width, height, channels;


		target.m_pData = stbi_load(
			path.c_str(),
			&width,
			&height,
			&channels,
			0
		);

		target.m_Width = (u32)width;
		target.m_Height = (u32)height;
		target.m_Channels = (u32)channels;


		QK_ASSERT(target.m_pData, std::format("Image `{}` failed to load: {}", path, stbi_failure_reason()));

		if (!target.m_pData) return false;

		return true;
	}

	void make_image(Image& target, u32 width, u32 height, u32 channels)
	{

		QK_ASSERT(target.m_pData == nullptr, "m_pData overwrite was attempted");
		
		QK_ASSERT(channels > 0 && channels <= 4, "channel count of range: expected value in [0:4]");

		size_t size = (size_t)width * (size_t)height * (size_t)channels;

		target.m_pData = (uint8_t*)malloc(size);

		QK_ASSERT(target.m_pData != nullptr, "m_pData was nullptr");

		if (target.m_pData == nullptr) return;

		memset(target.m_pData, 0, size);

		target.m_Width = width;
		target.m_Height = height;
		target.m_Channels = channels;
	}

}