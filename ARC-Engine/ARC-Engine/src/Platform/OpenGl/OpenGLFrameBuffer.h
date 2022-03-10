#pragma once
#include "ARC\Renderer\FrameBuffer.h"

namespace ARC {
	class COpenGLFrameBuffer : public CFrameBuffer
	{
	public:
		COpenGLFrameBuffer(const SFrameBufferSpecifications& _Specs);
		virtual ~COpenGLFrameBuffer();

		void Invalidate();

		virtual inline const SFrameBufferSpecifications& GetSpecifications() const override { return m_Specifications; };
		virtual inline uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; };

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void Resize(const TVec2<uint32_t>& _) override;

	private:
		uint32_t m_RendererID=0;
		uint32_t m_ColorAttachment=0, m_DepthAttachment=0;
		SFrameBufferSpecifications m_Specifications;
	};
}