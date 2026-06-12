#pragma once
#include "ARC\Renderer\FrameBuffer.h"

namespace arc {
	class opengl_framebuffer : public framebuffer
	{
	public:
		opengl_framebuffer(const frame_buffer_specification& pSpecs);
		virtual ~opengl_framebuffer();

		void Invalidate();

		virtual inline const frame_buffer_specification& get_specifications() const override { return specification_; };
		virtual inline u32 get_color_attachment_renderer_id(u32 pIndex=0) const override { ARC_CORE_ASSERT(pIndex<color_attachments_.size()) return color_attachments_[pIndex]; };

		virtual void bind() override;
		virtual void UnBind() override;

		virtual void resize(const vec2<u32>& p) override;

		virtual int read_pixel(u32 pAttachmentIndex, int pX, int pY) override;

    virtual void ClearColorAttachment(u32 pAttachmentIndex, const int pValue) override;
		virtual void ClearColorAttachment(u32 pAttachmentIndex, const float pValue) override;

	private:
		u32 renderer_id_=0;
		frame_buffer_specification specification_;

		std::vector<frame_buffer_texture_format> color_attachment_specifications_;
		frame_buffer_texture_format depth_attachment_specification_= frame_buffer_texture_format::None;

		std::vector<u32> color_attachments_;
		u32 depth_attachment_=0;

	};
}