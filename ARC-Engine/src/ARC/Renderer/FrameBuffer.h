#pragma once

#include "ARC.h"

namespace arc {
	
	enum class frame_buffer_texture_format : u32
	{
			None=0,

			// Color
			RGBA8,
			RED_INTEGER,

			// Depth/Stencil
			DEPTH24STENCIL8,

			// Defaults
			Depth = DEPTH24STENCIL8
	};

	struct frame_buffer_specification
	{
		u32 Width, Height;
		u32 Samples=1;

		std::vector<frame_buffer_texture_format> Attachments; // frame_buffer_texture_format
		
		u8 bSwapChainTarget : 1;
	};

	class framebuffer
	{
	public:
		virtual ~framebuffer() = default;

		virtual void bind()=0;
		virtual void UnBind()=0;
		
		virtual void resize(const vec2<u32>& p)=0;
		virtual int read_pixel(u32 pAttachmentIndex, int pX, int pY) = 0;

		virtual const frame_buffer_specification& get_specifications() const = 0;
		virtual u32 get_color_attachment_renderer_id(u32 pIndex = 0) const =0;
    
		virtual void ClearColorAttachment(u32 pAttachmentIndex, const int pValue) = 0;
		virtual void ClearColorAttachment(u32 pAttachmentIndex, const float pValue) = 0;

		static ref<framebuffer> create(const frame_buffer_specification& pSpec);

	};
}