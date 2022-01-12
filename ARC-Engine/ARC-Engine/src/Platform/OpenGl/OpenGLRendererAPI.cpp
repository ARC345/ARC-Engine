#include "arc_pch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
#include "ARC/Renderer/VertexArray.h"

namespace ARC {

	void COpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void COpenGLRendererAPI::SetClearColour(const glm::vec4 _Colour)
	{
		glClearColor(_Colour.r, _Colour.g, _Colour.b, _Colour.a);
	}

	void COpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void COpenGLRendererAPI::DrawIndexed(const TRef<CVertexArray>& _VertexArray)
	{
		glDrawElements(GL_TRIANGLES, _VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}