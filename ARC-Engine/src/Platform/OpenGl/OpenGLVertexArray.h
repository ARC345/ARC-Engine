#pragma once
#include "..\..\ARC\Renderer\VertexArray.h"

namespace arc {
class opengl_vertex_array : public vertex_array
{
	
public:
	opengl_vertex_array();
	virtual ~opengl_vertex_array();

	/*
	* @note Do not add vertex buffer before assigning a layout
	*/
	virtual void add_vertex_buffer(const ref<vertex_buffer>& vertex_buffer) override;
	virtual void set_index_buffer(const ref<index_buffer>& index_buffer) override;

	virtual const std::vector<ref<vertex_buffer>>& get_vertex_buffers() const override;
	virtual const ref<index_buffer>& get_index_buffer() const override;

	virtual void bind() const override;
	virtual void UnBind() const override;

private:
	uint32_t renderer_id_;
	std::vector<ref<vertex_buffer>> vertex_buffers_;
	ref<index_buffer> index_buffer_;
};
}