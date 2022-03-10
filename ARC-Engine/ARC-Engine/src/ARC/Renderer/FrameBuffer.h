#pragma once

#include "ARC.h"

namespace ARC {
	struct SFrameBufferSpecifications
	{
		uint32_t Width, Height;
		uint32_t Samples=1;

		uint8_t bSwapChainTarget : 1;
	};

	class CFrameBuffer
	{
	public:
		virtual ~CFrameBuffer() = default;

		virtual void Bind()=0;
		virtual void UnBind()=0;
		
		virtual void Resize(const TVec2<uint32_t>& _)=0;
		
		virtual const SFrameBufferSpecifications& GetSpecifications() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() const =0;

		static TRef<CFrameBuffer> Create(const SFrameBufferSpecifications& spec);

	};
}