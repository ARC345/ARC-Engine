#include "arc_pch.h"
#include "OpenGLFrameBuffer.h"
#include "glad\glad.h"

namespace ARC {
	static constexpr TUInt32 MaxFrameBufferSize = 10000;

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

	static GLenum ARCFBTextureFormatToGL(EFrameBufferTextureFormat pFormat)
	{
		switch (pFormat)
		{
			case EFrameBufferTextureFormat::RGBA8: return GL_RGBA8;
			case EFrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
		}

		ARC_CORE_ASSERT(false);
		return 0;
	}
	static GLenum GLDataType(EFrameBufferTextureFormat pFormat)
	{
		switch (pFormat)
		{
		case EFrameBufferTextureFormat::RGBA8: return GL_UNSIGNED_BYTE;
		case EFrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
		}

		ARC_CORE_ASSERT(false);
		return 0;
	}

	COpenGLFrameBuffer::COpenGLFrameBuffer(const SFrameBufferSpecification& pSpecs) :
		mSpecification(pSpecs)
	{
		for (EFrameBufferTextureFormat ispec : mSpecification.Attachments)
		{
			if (ispec != EFrameBufferTextureFormat::DEPTH24STENCIL8)
				mColorAttachmentSpecifications.emplace_back(ispec);
			else
				mDepthAttachmentSpecification = ispec;
		}

		Invalidate();
	}

	COpenGLFrameBuffer::~COpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &mRendererID);
		glDeleteTextures(mColorAttachmentSpecifications.size(), mColorAttachments.data());
		glDeleteTextures(1, &mDepthAttachment);
	}

	void COpenGLFrameBuffer::Invalidate()
	{
		if (mRendererID)
		{
			glDeleteFramebuffers(1, &mRendererID);
			glDeleteTextures(mColorAttachmentSpecifications.size(), mColorAttachments.data());
			glDeleteTextures(1, &mDepthAttachment);

			mColorAttachments.clear();
			mDepthAttachment=0;
		}

		glCreateFramebuffers(1, &mRendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
		
		bool bmultisample = mSpecification.Samples>1;

		if (mColorAttachmentSpecifications.size())
		{
			mColorAttachments.resize(mColorAttachmentSpecifications.size());
			CreateTextures(bmultisample, mColorAttachments.data(), (TUInt32)mColorAttachments.size());

			for (int i=0; i < mColorAttachments.size(); i++) {
				BindTexture(bmultisample, mColorAttachments[i]);

				switch (mColorAttachmentSpecifications[i]) {
					case EFrameBufferTextureFormat::RGBA8:
						AttachColorTexture(
							mColorAttachments[i],
							mSpecification.Samples,
							GL_RGBA8,
							GL_RGBA,
							mSpecification.Width,
							mSpecification.Height,
							i
						);
						break;
					case EFrameBufferTextureFormat::RED_INTEGER:
						AttachColorTexture(
							mColorAttachments[i],
							mSpecification.Samples,
							GL_R32I,
							GL_RED_INTEGER,
							mSpecification.Width,
							mSpecification.Height,
							i
						);
						break;
				}
			}
		}
		if (mDepthAttachmentSpecification != EFrameBufferTextureFormat::None)
		{
			CreateTextures(bmultisample, &mDepthAttachment, 1);
			BindTexture(bmultisample, mDepthAttachment);

			if (mDepthAttachmentSpecification == EFrameBufferTextureFormat::DEPTH24STENCIL8)
				AttachDepthTexture(
					mDepthAttachment,
					mSpecification.Samples,
					GL_DEPTH24_STENCIL8,
					GL_DEPTH_STENCIL_ATTACHMENT,
					mSpecification.Width,
					mSpecification.Height
				);
		}

		if (mColorAttachments.size()>1)
		{
				ARC_CORE_ASSERT(mColorAttachments.size()<=4, "Currently only 4 ColorAttachments Supported");

				GLenum buffers[4] = {
						GL_COLOR_ATTACHMENT0,
						GL_COLOR_ATTACHMENT1,
						GL_COLOR_ATTACHMENT2,
						GL_COLOR_ATTACHMENT3,
				};

				glDrawBuffers(mColorAttachments.size(), buffers);
		}
		else if (mColorAttachments.empty())
			glDrawBuffer(GL_NONE);

		ARC_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void COpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
		glViewport(0,0,mSpecification.Width, mSpecification.Height);
	}

	void COpenGLFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void COpenGLFrameBuffer::Resize(const TVec2<TUInt32>& p)
	{
		if (mSpecification.Width==0.f || mSpecification.Height==0.f || mSpecification.Height > MaxFrameBufferSize || mSpecification.Width > MaxFrameBufferSize) 
			return;

		mSpecification.Width = p.x;
		mSpecification.Height = p.y;
		Invalidate();
	}

	int COpenGLFrameBuffer::ReadPixel(TUInt32 pAttachmentIndex, int pX, int pY)
	{
		ARC_CORE_ASSERT(pAttachmentIndex<mColorAttachments.size());

		int pixelData;
		glReadBuffer(GL_COLOR_ATTACHMENT0+pAttachmentIndex);
		glReadPixels(pX,pY,1,1,GL_RED_INTEGER,GL_INT, &pixelData);

		return pixelData;
	}
	void COpenGLFrameBuffer::ClearColorAttachment(TUInt32 pAttachmentIndex, const int pValue)
	{
		ARC_CORE_ASSERT(pAttachmentIndex < mColorAttachments.size());

		glClearTexImage(
			mColorAttachments[pAttachmentIndex],
			0,
			ARCFBTextureFormatToGL(mColorAttachmentSpecifications[pAttachmentIndex]),
			GL_INT,
			&pValue
		);
	}
	void COpenGLFrameBuffer::ClearColorAttachment(TUInt32 pAttachmentIndex, const float pValue)
	{
		ARC_CORE_ASSERT(pAttachmentIndex < mColorAttachments.size());

		glClearTexImage(
			mColorAttachments[pAttachmentIndex],
			0,
			ARCFBTextureFormatToGL(mColorAttachmentSpecifications[pAttachmentIndex]),
			GL_FLOAT,
			&pValue
		);
	}
}