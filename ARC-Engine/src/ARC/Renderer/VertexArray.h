#pragma once
#include "ARC\Renderer\Buffer.h"
#include <memory>

namespace arc {
	class vertex_array
	{
	public:
		virtual ~vertex_array() = default;
		static ref<vertex_array> create();

		virtual void add_vertex_buffer(const ref<vertex_buffer>& vertex_buffer) = 0;
		virtual void set_index_buffer(const ref<index_buffer>& index_buffer) = 0;
		
		virtual const std::vector<ref<vertex_buffer>>& get_vertex_buffers() const = 0;
		virtual const ref<index_buffer>& get_index_buffer() const = 0;

		virtual void bind() const = 0;
		virtual void UnBind() const = 0;
	};
}