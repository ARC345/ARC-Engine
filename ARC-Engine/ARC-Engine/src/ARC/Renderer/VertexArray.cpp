#include "arc_pch.h"
#include "VertexArray.h"
#include "..\..\Platform\OpenGl\OpenGLVertexArray.h"
#include "Renderer.h"

namespace ARC {

	TRef<CVertexArray> CVertexArray::Create()
	{
		switch (CRenderer::GetCurrentAPI())
		{
		case CRendererAPI::ERendererAPI::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
			return nullptr;
		case CRendererAPI::ERendererAPI::OpenGL:
			return std::make_shared<COpenGLVertexArray>();
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	void CVertexArray::AddVertexBuffer(const TRef<CVertexBuffer>& _VertexBuffer)
	{

	}

	void CVertexArray::SetIndexBuffer(const TRef<CIndexBuffer>& _IndexBuffer)
	{

	}

	void CVertexArray::Bind() const
	{

	}

	void CVertexArray::UnBind() const
	{

	}

}