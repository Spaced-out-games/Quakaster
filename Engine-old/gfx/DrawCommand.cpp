#include "DrawCommand.h"
#include "GL/glew.h"
#include <bit>

namespace qk::gfx {
	/*
	
		command.handles[0] = primitive_t;
		command.handles[1] = count;
		command.handles[2] = type;
		command.handles[3] = offsetBytes;
	*/

	void call(const DrawCommand& command) {
		using enum ECommandType;
		float r, g, b, a;
		switch (command.type)
		{
		case BIND_VERTEXARRAY:
			glBindVertexArray(command.handles[0]);
			break;
		case BIND_VERTEXBUFFER:
			glBindBuffer(GL_ARRAY_BUFFER, command.handles[0]);
			break;

		case BIND_INDEXBUFFER:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, command.handles[0]);
			break;

		case BIND_TEXTURE:
			glActiveTexture(GL_TEXTURE0 + command.handles[2]); // slot
			glBindTexture(command.handles[1], command.handles[0]); // type, handle
			break;
		case BIND_SHADER:
			glUseProgram(command.handles[0]);
			break;
		case SET_UNIFORM_INT:
			glUniform1i(command.handles[0], std::bit_cast<int>(command.handles[1]));
			break;

		case DRAW_VERTEXARRAY:
			glDrawArrays(command.handles[0], command.handles[1], command.handles[2]);
			break;
		case DRAW_ELEMENTS:
			glDrawElements(
				command.handles[0],
				command.handles[1],
				command.handles[2],
				(void*)command.handles[3]
			);
			break;
		case DRAW_ARRAYS_INSTANCED:
			glDrawArraysInstanced(command.handles[0], command.handles[1], command.handles[2], command.handles[3]);
			break;
		case CLEAR:
			r = std::bit_cast<float>(command.handles[1]);
			g = std::bit_cast<float>(command.handles[2]);
			b = std::bit_cast<float>(command.handles[3]);
			a = std::bit_cast<float>(command.handles[4]);

			glClearColor(r, g, b, a);
			glClear(command.handles[0]);
			break;
		case BIND_FRAMEBUFFER:
			glBindFramebuffer(GL_FRAMEBUFFER, command.handles[0]);
			break;
		case SET_VIEWPORT:
			glViewport(command.handles[0], command.handles[1], command.handles[2], command.handles[3]);
			break;
		default:
			break;
		}

	}

}