#include "arc_pch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
#include "ARC/Renderer/VertexArray.h"

namespace ARC {

	void COpenGLRendererAPI::SetClearColour(const glm::vec4 _Colour)
	{
		glClearColor(_Colour.r, _Colour.g, _Colour.b, _Colour.a);
	}

	void COpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void COpenGLRendererAPI::DrawIndexed(const std::shared_ptr<CVertexArray>& _VertexArray)
	{
		glDrawElements(GL_TRIANGLES, _VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}