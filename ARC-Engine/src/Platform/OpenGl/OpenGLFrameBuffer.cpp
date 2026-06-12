#include "arc_pch.h"
#include "OpenGLFrameBuffer.h"
#include "glad\glad.h"

namespace arc {
	static constexpr u32 MaxFrameBufferSize = 10000;

	static void CreateTextures(bool pbMultisampled, uint32_t* pOutID, uint32_t pCount)
	{
		glCreateTextures(
			pbMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
			pCount,
			pOutID
			);
	}

	static void BindTexture(bool pbMultisampled, uint32_t pId)
	{
		glBindTexture(
			pbMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
			pId
			);
	}

	static void AttachColorTexture(uint32_t pId, int pSamples, GLenum pInternalFormat, GLenum pFormat, uint32_t pWidth, uint32_t pHeight, int pIndex)
	{
		bool pbMultisampled = pSamples > 1;
		if (pbMultisampled)
		{
			glTexImage2DMultisample(
				GL_TEXTURE_2D_MULTISAMPLE,
				pSamples,
				pInternalFormat,
				pWidth,
				pHeight,
				GL_FALSE
				);
		}
		else
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				pInternalFormat,
				pWidth,
				pHeight,
				0,
				pFormat,
				GL_UNSIGNED_BYTE,
				nullptr
				);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + pIndex,
			pbMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
			pId,
			0
			);
	}

	static void AttachDepthTexture(uint32_t pId, int pSamples, GLenum pFormat, GLenum pAttachmentType, uint32_t pWidth, uint32_t pHeight)
	{
		bool pbMultisampled = pSamples > 1;
		if (pbMultisampled)
		{
			glTexImage2DMultisample(
				GL_TEXTURE_2D_MULTISAMPLE,
				pSamples,
				pFormat,
				pWidth,
				pHeight,
				GL_FALSE
				);
		}
		else
		{
			glTexStorage2D(
				GL_TEXTURE_2D,
				1,
				pFormat,
				pWidth,
				pHeight
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			pAttachmentType,
			pbMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
			pId,
			0
			);
	}

	static GLenum ARCFBTextureFormatToGL(frame_buffer_texture_format pFormat)
	{
		switch (pFormat)
		{
			case frame_buffer_texture_format::RGBA8: return GL_RGBA8;
			case frame_buffer_texture_format::RED_INTEGER: return GL_RED_INTEGER;
		}

		ARC_CORE_ASSERT(false);
		return 0;
	}
	static GLenum GLDataType(frame_buffer_texture_format pFormat)
	{
		switch (pFormat)
		{
		case frame_buffer_texture_format::RGBA8: return GL_UNSIGNED_BYTE;
		case frame_buffer_texture_format::RED_INTEGER: return GL_RED_INTEGER;
		}

		ARC_CORE_ASSERT(false);
		return 0;
	}

	opengl_framebuffer::opengl_framebuffer(const frame_buffer_specification& pSpecs) :
		specification_(pSpecs)
	{
		for (frame_buffer_texture_format ispec : specification_.Attachments)
		{
			if (ispec != frame_buffer_texture_format::DEPTH24STENCIL8)
				color_attachment_specifications_.emplace_back(ispec);
			else
				depth_attachment_specification_ = ispec;
		}

		Invalidate();
	}

	opengl_framebuffer::~opengl_framebuffer()
	{
		glDeleteFramebuffers(1, &renderer_id_);
		glDeleteTextures(color_attachment_specifications_.size(), color_attachments_.data());
		glDeleteTextures(1, &depth_attachment_);
	}

	void opengl_framebuffer::Invalidate()
	{
		if (renderer_id_)
		{
			glDeleteFramebuffers(1, &renderer_id_);
			glDeleteTextures(color_attachment_specifications_.size(), color_attachments_.data());
			glDeleteTextures(1, &depth_attachment_);

			color_attachments_.clear();
			depth_attachment_=0;
		}

		glCreateFramebuffers(1, &renderer_id_);
		glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);
		
		bool bmultisample = specification_.Samples>1;

		if (color_attachment_specifications_.size())
		{
			color_attachments_.resize(color_attachment_specifications_.size());
			CreateTextures(bmultisample, color_attachments_.data(), (u32)color_attachments_.size());

			for (int i=0; i < color_attachments_.size(); i++) {
				BindTexture(bmultisample, color_attachments_[i]);

				switch (color_attachment_specifications_[i]) {
					case frame_buffer_texture_format::RGBA8:
						AttachColorTexture(
							color_attachments_[i],
							specification_.Samples,
							GL_RGBA8,
							GL_RGBA,
							specification_.Width,
							specification_.Height,
							i
						);
						break;
					case frame_buffer_texture_format::RED_INTEGER:
						AttachColorTexture(
							color_attachments_[i],
							specification_.Samples,
							GL_R32I,
							GL_RED_INTEGER,
							specification_.Width,
							specification_.Height,
							i
						);
						break;
				}
			}
		}
		if (depth_attachment_specification_ != frame_buffer_texture_format::None)
		{
			CreateTextures(bmultisample, &depth_attachment_, 1);
			BindTexture(bmultisample, depth_attachment_);

			if (depth_attachment_specification_ == frame_buffer_texture_format::DEPTH24STENCIL8)
				AttachDepthTexture(
					depth_attachment_,
					specification_.Samples,
					GL_DEPTH24_STENCIL8,
					GL_DEPTH_STENCIL_ATTACHMENT,
					specification_.Width,
					specification_.Height
				);
		}

		if (color_attachments_.size()>1)
		{
				ARC_CORE_ASSERT(color_attachments_.size()<=4, "Currently only 4 ColorAttachments Supported");

				GLenum buffers[4] = {
						GL_COLOR_ATTACHMENT0,
						GL_COLOR_ATTACHMENT1,
						GL_COLOR_ATTACHMENT2,
						GL_COLOR_ATTACHMENT3,
				};

				glDrawBuffers(color_attachments_.size(), buffers);
		}
		else if (color_attachments_.empty())
			glDrawBuffer(GL_NONE);

		ARC_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void opengl_framebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);
		glViewport(0,0,specification_.Width, specification_.Height);
	}

	void opengl_framebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void opengl_framebuffer::resize(const vec2<u32>& p)
	{
		if (specification_.Width==0.f || specification_.Height==0.f || specification_.Height > MaxFrameBufferSize || specification_.Width > MaxFrameBufferSize) 
			return;

		specification_.Width = p.x;
		specification_.Height = p.y;
		Invalidate();
	}

	int opengl_framebuffer::read_pixel(u32 pAttachmentIndex, int pX, int pY)
	{
		ARC_CORE_ASSERT(pAttachmentIndex<color_attachments_.size());

		int pixelData;
		glReadBuffer(GL_COLOR_ATTACHMENT0+pAttachmentIndex);
		glReadPixels(pX,pY,1,1,GL_RED_INTEGER,GL_INT, &pixelData);

		return pixelData;
	}
	void opengl_framebuffer::ClearColorAttachment(u32 pAttachmentIndex, const int pValue)
	{
		ARC_CORE_ASSERT(pAttachmentIndex < color_attachments_.size());

		glClearTexImage(
			color_attachments_[pAttachmentIndex],
			0,
			ARCFBTextureFormatToGL(color_attachment_specifications_[pAttachmentIndex]),
			GL_INT,
			&pValue
		);
	}
	void opengl_framebuffer::ClearColorAttachment(u32 pAttachmentIndex, const float pValue)
	{
		ARC_CORE_ASSERT(pAttachmentIndex < color_attachments_.size());

		glClearTexImage(
			color_attachments_[pAttachmentIndex],
			0,
			ARCFBTextureFormatToGL(color_attachment_specifications_[pAttachmentIndex]),
			GL_FLOAT,
			&pValue
		);
	}
}