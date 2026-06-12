#pragma once
#include "ARC\Renderer\Buffer.h"

namespace arc {
	class opengl_vertex_buffer : public vertex_buffer
	{
	public:
		opengl_vertex_buffer(float* pVertices, u32 pSize);
		opengl_vertex_buffer(u32 pSize);
		virtual ~opengl_vertex_buffer();

		virtual void bind() const override;
		virtual void UnBind() const override;

		virtual void set_layout(const buffer_layout& pLayout) override { layout_ = pLayout; }
		virtual const buffer_layout& get_layout() const override { return layout_; }

		virtual void set_data(const void* pData, u32 pSize) override;

	private:
		u32 renderer_id_;
		buffer_layout layout_;
	};
	
	class opengl_index_buffer : public index_buffer
	{
	public:
		opengl_index_buffer(u32* pIndiices, u32 pCount);
		virtual ~opengl_index_buffer();

		virtual void bind() const override;
		virtual void UnBind() const override;

		virtual u32 get_count() const override;

	private:
		u32 renderer_id_;
		u32 count_;
	};
}