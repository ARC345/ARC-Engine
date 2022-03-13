#include "arc_pch.h"
#include "FrameBuffer.h"
#include "Platform\OpenGl\OpenGLFrameBuffer.h"

namespace ARC {

	TRef<CFrameBuffer> CFrameBuffer::Create(const SFrameBufferSpecifications& spec)
	{
		switch (CRenderer::GetCurrentAPI())
		{
		case CRendererAPI::ERendererAPI::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
			return nullptr;
		case CRendererAPI::ERendererAPI::OpenGL:
			return CreateRef<COpenGLFrameBuffer>(spec);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

}