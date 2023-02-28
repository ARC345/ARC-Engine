#pragma once
#include "ARC\Renderer\FrameBuffer.h"

namespace ARC {
	class COpenGLFrameBuffer : public CFrameBuffer
	{
	public:
		COpenGLFrameBuffer(const SFrameBufferSpecification& pSpecs);
		virtual ~COpenGLFrameBuffer();

		void Invalidate();

		virtual inline const SFrameBufferSpecification& GetSpecifications() const override { return mSpecification; };
		virtual inline TUInt32 GetColorAttachmentRendererID(TUInt32 pIndex=0) const override { ARC_CORE_ASSERT(pIndex<mColorAttachments.size()) return mColorAttachments[pIndex]; };

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void Resize(const TVec2<TUInt32>& p) override;

		virtual int ReadPixel(TUInt32 pAttachmentIndex, int pX, int pY) override;

	private:
		TUInt32 mRendererID=0;
		SFrameBufferSpecification mSpecification;

		std::vector<EFrameBufferTextureFormat> mColorAttachmentSpecifications;
		EFrameBufferTextureFormat mDepthAttachmentSpecification= EFrameBufferTextureFormat::None;

		std::vector<TUInt32> mColorAttachments;
		TUInt32 mDepthAttachment=0;

	};
}