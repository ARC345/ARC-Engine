#include "arc_pch.h"
#include "OpenGLVertexArray.h"
#include "glad\glad.h"
#include "ARC\Renderer\Buffer.h"

namespace arc {
	struct SOpenGLShaderHelper {
		static GLenum ShaderTypeToOpenGlBaseType(shader_data_type type) {
			switch (type)
			{
			case shader_data_type::Float:	return GL_FLOAT;
			case shader_data_type::Float2:	return GL_FLOAT;
			case shader_data_type::Float3:	return GL_FLOAT;
			case shader_data_type::Float4:	return GL_FLOAT;
			case shader_data_type::Mat3:		return GL_FLOAT;
			case shader_data_type::Mat4:		return GL_FLOAT;
			case shader_data_type::Int:		return GL_INT;
			case shader_data_type::Int2:		return GL_INT;
			case shader_data_type::Int3:		return GL_INT;
			case shader_data_type::Int4:		return GL_INT;
			case shader_data_type::Bool:		return GL_BOOL;
			}

			ARC_CORE_ASSERT(false, "UnknownShaderDataType");
				return 0;
		};
	};

	opengl_vertex_array::opengl_vertex_array()
	{
		glCreateVertexArrays(1, &renderer_id_);
	}

	opengl_vertex_array::~opengl_vertex_array()
	{
		glDeleteVertexArrays(1, &renderer_id_);
	}
	
	void opengl_vertex_array::add_vertex_buffer(const ref<vertex_buffer>& vertex_buffer)
	{
		bind();
		vertex_buffer->bind();

		ARC_CORE_ASSERT(vertex_buffer->get_layout().get_elements().size(), "Vertex Buffer has no layout!");

		uint32_t index = 0;
		const auto& layout = vertex_buffer->get_layout();
		for (const auto& ielement : layout)
		{
			switch (ielement.Type)
			{
			case shader_data_type::Float:
			case shader_data_type::Float2:
			case shader_data_type::Float3:
			case shader_data_type::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					shader_helper::get_component_count(ielement.Type),
					SOpenGLShaderHelper::ShaderTypeToOpenGlBaseType(ielement.Type),
					ielement.bNormalized ? GL_TRUE : GL_FALSE,
					layout.get_stride(),
					(const void*)(const u64)ielement.Offset);
				index++;
				break;
			}
			case shader_data_type::Int:
			case shader_data_type::Int2:
			case shader_data_type::Int3:
			case shader_data_type::Int4:
			case shader_data_type::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index,
					shader_helper::get_component_count(ielement.Type),
					SOpenGLShaderHelper::ShaderTypeToOpenGlBaseType(ielement.Type),
					layout.get_stride(),
					(const void*)(const u64)ielement.Offset);
				index++;
				break;
			}
			case shader_data_type::Mat3:
			case shader_data_type::Mat4:
			{
				uint8_t count = shader_helper::get_component_count(ielement.Type);
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						count,
						SOpenGLShaderHelper::ShaderTypeToOpenGlBaseType(ielement.Type),
						ielement.bNormalized ? GL_TRUE : GL_FALSE,
						layout.get_stride(),
						(const void*)(ielement.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
			default:
				ARC_CORE_ASSERT(false, "Unknown shader_data_type!");
			}
		}

		vertex_buffers_.push_back(vertex_buffer);
	}

	void opengl_vertex_array::set_index_buffer(const ref<index_buffer>& index_buffer)
	{
		bind();
		index_buffer->bind();

		index_buffer_ = index_buffer;
	}

	const std::vector<ref<vertex_buffer>>& opengl_vertex_array::get_vertex_buffers() const
	{
		return vertex_buffers_;
	}

	const ref<index_buffer>& opengl_vertex_array::get_index_buffer() const
	{
		return index_buffer_;
	}
	void opengl_vertex_array::bind() const
	{
		glBindVertexArray(renderer_id_);
	}

	void opengl_vertex_array::UnBind() const
	{
		glBindVertexArray(0);
	}

}