#include "arc_pch.h"
#include "Renderer.h"
#include "Platform\OpenGl\OpenGLShader.h"
#include "ARC\Core\Core.h"

namespace ARC {
	CShader* CShader::Create(const std::string& _VertexSrc, const std::string& _FragmentSrc)
	{
		switch (CRenderer::GetCurrentAPI())
		{
		case CRendererAPI::ERendererAPI::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
			return nullptr;
		case CRendererAPI::ERendererAPI::OpenGL:
			return new COpenGLShader(_VertexSrc, _FragmentSrc);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}
}