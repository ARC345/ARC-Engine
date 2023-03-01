#pragma once

#include "ARC.h"

namespace ARC {
	
	enum class EFrameBufferTextureFormat : TUInt32
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

	struct SFrameBufferSpecification
	{
		TUInt32 Width, Height;
		TUInt32 Samples=1;

		std::vector<EFrameBufferTextureFormat> Attachments; // EFrameBufferTextureFormat
		
		TUInt8 bSwapChainTarget : 1;
	};

	class CFrameBuffer
	{
	public:
		virtual ~CFrameBuffer() = default;

		virtual void Bind()=0;
		virtual void UnBind()=0;
		
		virtual void Resize(const TVec2<TUInt32>& p)=0;
		virtual int ReadPixel(TUInt32 pAttachmentIndex, int pX, int pY) = 0;

		virtual const SFrameBufferSpecification& GetSpecifications() const = 0;
		virtual TUInt32 GetColorAttachmentRendererID(TUInt32 pIndex = 0) const =0;

		virtual void ClearColorAttachment(TUInt32 pAttachmentIndex, const int pValue) = 0;
		virtual void ClearColorAttachment(TUInt32 pAttachmentIndex, const float pValue) = 0;

		static TRef<CFrameBuffer> Create(const SFrameBufferSpecification& pSpec);

	};
}