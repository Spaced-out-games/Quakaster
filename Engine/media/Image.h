/// **************************************** QUAKASTER ENGINE **************************************** 
/// media/Image.h
/// Purpose: Defines a structure that can load images from disk
/// ************************************************************************************************** 
#pragma once
#include "core.h"
#include <stdint.h>
#include <string>
#include <stdexcept>
#include <cstring>
#include <cstdlib>



namespace qk::media {
	class QK_API Image {
		u8* m_pData = nullptr;
		u32 m_Width = 0;
		u32 m_Height = 0;
		u32 m_Channels = 0;
		public:

			Image() = default;
			Image(const std::string& path);
			Image(u32 width, u32 height, u32 channels);


			// Hot-reloads the image
			void reload(const std::string& path);

			// Sets all pixels to black
			void clear();


			~Image();

			Image(const Image&) = delete;
			Image& operator=(const Image&) = delete;

			Image(Image&& other) noexcept;

			Image& operator=(Image&& other) noexcept;

			u8* data() const;
			u32 width() const;
			u32 height() const;
			u32 channels() const;
			size_t size_bytes() const;
			void destroy();
			friend bool load_image(Image& target, const std::string& path);
			friend void make_image(Image& target, u32 width, u32 height, u32 channels);
	};


	/// <summary>
	/// Loads an image from `path`. Assumes `target` has already been destroyed
	/// </summary>
	/// <param name="target"></param>
	/// <param name="path"></param>
	bool load_image(Image& target, const std::string& path);

	/// <summary>
	/// Creates an empty (black) image. Assumes `target` has already been destroyed.
	/// </summary>
	/// <param name="target"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="channels"></param>
	void make_image(Image& target, u32 width, u32 height, u32 channels);

}