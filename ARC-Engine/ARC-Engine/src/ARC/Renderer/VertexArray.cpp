#include "arc_pch.h"
#include "VertexArray.h"
#include "..\..\Platform\OpenGl\OpenGLVertexArray.h"
#include "Renderer.h"

namespace ARC {

	CVertexArray* CVertexArray::Create()
	{
		switch (CRenderer::GetCurrentAPI())
		{
		case CRendererAPI::ERendererAPI::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
			return nullptr;
		case CRendererAPI::ERendererAPI::OpenGL:
			return new COpenGLVertexArray();
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	void CVertexArray::AddVertexBuffer(const std::shared_ptr<CVertexBuffer>& _VertexBuffer)
	{

	}

	void CVertexArray::SetIndexBuffer(const std::shared_ptr<CIndexBuffer>& _IndexBuffer)
	{

	}

	void CVertexArray::Bind() const
	{

	}

	void CVertexArray::UnBind() const
	{

	}

}