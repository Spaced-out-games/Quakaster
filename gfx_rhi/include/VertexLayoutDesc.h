#pragma once
#include "core.h"
#include "Handle.h"
#include "Scalar.h"

using qk::Handle;

namespace qk::gfx::rhi {
	/* Core calls:
	glEnableVertexAttribArray(elem.location);


	enable the generic vertex attribute array specified by location
	void glEnableVertexAttribArray(GLuint location);
		location
		Specifies the location of the generic vertex attribute to be enabled or disabled.


	specify the location and data format of the array of generic vertex attributes at `location` to use when rendering.
	void glVertexAttribPointer(	GLuint location, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
		location
			Specifies the index of the generic vertex attribute to be modified.

		size
			Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4. Additionally, the symbolic constant GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.

		type
			Specifies the data type of each component in the array. The symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted by glVertexAttribPointer and glVertexAttribIPointer. Additionally GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are accepted by glVertexAttribPointer. GL_DOUBLE is also accepted by glVertexAttribLPointer and is the only token accepted by the type parameter for that function. The initial value is GL_FLOAT.

		normalized
			For glVertexAttribPointer, specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.

		stride
			Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.

		pointer
			Specifies the offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
	
	specify the location and data format of the array of generic vertex attributes at location` to use when rendering.
	void glVertexAttribIPointer( GLuint location, GLint size, GLenum type, GLsizei stride, const void* pointer);
	


	modifies the rate at which generic vertex attributes advance when rendering multiple instances of primitives in a single draw call. If `divisor` is zero, the attribute at slot `location` advances once per vertex.
	If divisor is non-zero, the attribute advances once per `divisor` instances of the set(s) of vertices being rendered.
	void glVertexAttribDivisor(	GLuint location, GLuint divisor);
	location
		Specify the index of the generic vertex attribute.

	divisor
		Specify the number of instances that will pass between updates of the generic attribute at slot `location`.

	*/

	enum InputRate : uint8_t {
		PerVertex,
		PerInstance
	};



	struct VertexAttribute_createinfo {
		uint32_t location;
		ComponentDesc component;
		uintptr_t offset;

	};
	
	// uint32_t binding; // artificial construct in GL; inferred by array index
	struct VertexAttributeBinding {
		InputRate input_rate; // the divisor
		uint32_t stride;

		std::vector<VertexAttribute_createinfo> attributes;

	};

	using VertexBinding = uint32_t;

	struct VertexAttributePreset {
		EScalarType type;
		uint32_t count;
		EScalarMultiplicity multiplicity;
		bool normalized;
	};

	class VertexLayout_createinfo {
		std::vector<VertexAttributeBinding> m_Bindings;

		public:
			VertexBinding pushBinding(InputRate rate, uint32_t stride = 0) {
				VertexBinding binding = (VertexBinding)(m_Bindings.size());

				VertexAttributeBinding newBinding{};
				newBinding.input_rate = rate;
				newBinding.stride = stride;

				m_Bindings.push_back(std::move(newBinding));

				return binding;
			}
			void push(
				VertexBinding binding,
				uint32_t location,
				uintptr_t offset,
				EScalarType comp_type,
				uint32_t comp_count,
				EScalarMultiplicity comp_multiplicity,
				bool comp_normalize
			) {
				QK_ASSERT(!m_Bindings.empty());

				QK_ASSERT(binding < m_Bindings.size());

				auto& target = m_Bindings[binding];

				target.attributes.emplace_back(location, ComponentDesc{ comp_multiplicity, comp_normalize, comp_count, comp_type }, offset);

			}



			VertexLayout_createinfo& push(VertexBinding binding, u32 location, uintptr_t offset, const VertexAttributePreset& preset) {
				push(binding, location, offset, preset.type, preset.count, preset.multiplicity, preset.normalized);
				return *this;
			}

			template <class T>
			VertexLayout_createinfo& push (
				VertexBinding binding, u32 location, u32 comp_offset, u32 comp_count,
				EScalarMultiplicity comp_multiplicity, bool comp_normalize) {
				


				EScalarType comp_type = EScalarType_cast<T>();
				push(binding, location, comp_offset, comp_type, comp_count, comp_multiplicity, comp_normalize);
				return *this;
			}

			const std::vector<VertexAttributeBinding>& bindings() {
				return m_Bindings;
			}




	};


	namespace layout {

		constexpr VertexAttributePreset vec2f = {
			.type = EScalarType::f32,
			.count = 2,
			.multiplicity = Vector,
			.normalized = false
		};


		constexpr VertexAttributePreset vec3f = {
			.type = EScalarType::f32,
			.count = 3,
			.multiplicity = Vector,
			.normalized = false
		};

		constexpr VertexAttributePreset vec4f = {
			.type = EScalarType::f32,
			.count = 4,
			.multiplicity = Vector,
			.normalized = false
		};
	}



}