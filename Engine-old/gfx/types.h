/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/types.h
/// Purpose: Defines some core gfx:: types
/// **************************************************************************************************


#pragma once
#include "GL/glew.h"

namespace qk::gfx {
	using Handle = unsigned int;

	using TextureType = unsigned int;

	static constexpr Handle NULL_HANDLE = 0;

	using VertexAttribute = unsigned int;

	using PrimitiveType = unsigned int;

	template<typename>
	inline constexpr bool VertexTypeImplemented = false;

	template <class T>
	VertexAttribute VertexAttribute_cast();


}


#include "types_GL.inl"