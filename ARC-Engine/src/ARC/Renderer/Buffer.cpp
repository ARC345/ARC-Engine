#include "arc_pch.h"
#include "Buffer.h"
#include "..\Renderer\Renderer.h"
#include "Platform\OpenGl\OpenGLBuffer.h"
#include "..\Core\Core.h"

namespace ARC {

	TRef<CVertexBuffer> CVertexBuffer::Create(float* _Vertices, uint32_t _Size)
	{
		switch (CRenderer::GetCurrentAPI())
		{
			case CRendererAPI::ERendererAPI::None:
				ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
				return nullptr;
			case CRendererAPI::ERendererAPI::OpenGL:
				return std::make_shared<COpenGLVertexBuffer>(_Vertices, _Size);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	TRef<CVertexBuffer> CVertexBuffer::Create(uint32_t _Size)
	{
		switch (CRenderer::GetCurrentAPI())
		{
		case CRendererAPI::ERendererAPI::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
			return nullptr;
		case CRendererAPI::ERendererAPI::OpenGL:
			return std::make_shared<COpenGLVertexBuffer>(_Size);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	TRef<CIndexBuffer> CIndexBuffer::Create(uint32_t* _Indices, uint32_t _Count)
	{
		switch (CRenderer::GetCurrentAPI())
		{
		case CRendererAPI::ERendererAPI::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
			return nullptr;
		case CRendererAPI::ERendererAPI::OpenGL:
			return std::make_shared<COpenGLIndexBuffer>(_Indices, _Count);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}
}