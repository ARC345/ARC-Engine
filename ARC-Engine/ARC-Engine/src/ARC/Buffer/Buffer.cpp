#include "arc_pch.h"
#include "Buffer.h"
#include "..\Renderer\Renderer.h"
#include "Platform\OpenGl\OpenGLBuffer.h"

namespace ARC {

	CVertexBuffer* CVertexBuffer::Create(float* _Vertices, uint32_t _Size)
	{
		switch (CRenderer::GetCurrentAPI())
		{
			case CRendererAPI::ERendererAPI::None:
				ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
				return nullptr;
			case CRendererAPI::ERendererAPI::OpenGL:
				return new COpenGLVertexBuffer(_Vertices, _Size);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	CIndexBuffer* CIndexBuffer::Create(uint32_t* _Indices, uint32_t _Count)
	{
		switch (CRenderer::GetCurrentAPI())
		{
		case CRendererAPI::ERendererAPI::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
			return nullptr;
		case CRendererAPI::ERendererAPI::OpenGL:
			return new COpenGLIndexBuffer(_Indices, _Count);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}
}