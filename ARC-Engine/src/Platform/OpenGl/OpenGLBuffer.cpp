#include "arc_pch.h"
#include "OpenGLBuffer.h"
#include "glad\glad.h"

namespace arc {
	//---------------------------[vertex_buffer]----------------------------//
	
	opengl_vertex_buffer::opengl_vertex_buffer(uint32_t size)
	{
		glCreateBuffers(1, &renderer_id_);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	opengl_vertex_buffer::opengl_vertex_buffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &renderer_id_);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	opengl_vertex_buffer::~opengl_vertex_buffer()
	{
		glDeleteBuffers(1, &renderer_id_);
	}

	void opengl_vertex_buffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
	}

	void opengl_vertex_buffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void opengl_vertex_buffer::set_data(const void* data, uint32_t size)
	{
		bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	//---------------------------[index_buffer]----------------------------//

	opengl_index_buffer::opengl_index_buffer(uint32_t* indiices, uint32_t count)
		: count_(count)
	{
		glCreateBuffers(1, &renderer_id_);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indiices, GL_STATIC_DRAW);
	}

	opengl_index_buffer::~opengl_index_buffer()
	{
		glDeleteBuffers(1, &renderer_id_);
	}

	void opengl_index_buffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
	}

	void opengl_index_buffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t opengl_index_buffer::get_count() const
	{
		return count_;
	}
}
