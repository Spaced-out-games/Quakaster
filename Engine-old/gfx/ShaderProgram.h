/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/ShaderProgram.h
/// Purpose: Represents a fully compiled Shader program, ready to go!
/// **************************************************************************************************



#pragma once
#include "Shader.h"
#include "../core.h"
#include "types.h"

namespace qk::gfx {
	class QK_API ShaderProgram {
		Handle m_Handle = NULL_HANDLE;

		public:
			// todo: somehow manage double initialization
			bool init(const Handle frag, const Handle vert);

			void bind();

			static void unbind();

			Handle handle() const;

			void destroy();

			gfx::Handle uniform(const std::string& name);
			~ShaderProgram();

	};
}