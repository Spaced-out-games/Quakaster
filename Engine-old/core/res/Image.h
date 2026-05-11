/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/res/Image.h
/// Purpose: Defines a structure that can load images from disk
/// ************************************************************************************************** 
#pragma once
#include "../../core.h"
#include <stdint.h>
#include <string>
#include <stdexcept>
#include <cstring>
#include <cstdlib>



namespace qk::resource {
	class QK_API Image {
		uint8_t* m_Data = nullptr;
		int m_Width = 0;
		int m_Height = 0;
		int m_Channels = 0;
		public:

			Image() = default;
			Image(const std::string& path);
			Image(int width, int height, int channels);


			// Hot-reloads the image
			void reload(const std::string& path);

			// Sets all pixels to black
			void clear();


			~Image();

			Image(const Image&) = delete;
			Image& operator=(const Image&) = delete;

			Image(Image&& other) noexcept;

			Image& operator=(Image&& other) noexcept;

			uint8_t* data() const;
			int width() const;
			int height() const;
			int channels() const;
			size_t size_bytes() const;
			void destroy();
			friend void load_image(Image& target, const std::string& path);
			friend void make_image(Image& target, int width, int height, int channels);
	};


	/// <summary>
	/// Loads an image from `path`. Assumes `target` has already been destroyed
	/// </summary>
	/// <param name="target"></param>
	/// <param name="path"></param>
	void load_image(Image& target, const std::string& path);

	/// <summary>
	/// Creates an empty (black) image. Assumes `target` has already been destroyed.
	/// </summary>
	/// <param name="target"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="channels"></param>
	void make_image(Image& target, int width, int height, int channels);

}