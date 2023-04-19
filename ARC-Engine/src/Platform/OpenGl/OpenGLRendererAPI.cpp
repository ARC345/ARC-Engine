#include "arc_pch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
#include "ARC/Renderer/VertexArray.h"

namespace ARC {

	void COpenGLRendererAPI::Init()
	{
		ARC_PROFILE_FUNCTION();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_DEPTH_TEST);
	}

	void COpenGLRendererAPI::SetViewport(const TVec2<TUInt32> _BottemLeftCoord, const TVec2<TUInt32> _Dimentions)
	{
		glViewport(_BottemLeftCoord.x, _BottemLeftCoord.y, _Dimentions.x, _Dimentions.y);
	}

	void COpenGLRendererAPI::SetClearColour(const FColor4 _Colour)
	{
		glClearColor(_Colour.r, _Colour.g, _Colour.b, _Colour.a);
	}

	void COpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void COpenGLRendererAPI::DrawIndexed(const TRef<CVertexArray>& _VertexArray, TUInt32 _Count)
	{
		ARC_PROFILE_FUNCTION();
		_VertexArray->Bind();
		TUInt32 count = _Count ? _Count : _VertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void COpenGLRendererAPI::DrawLine(const TRef<CVertexArray>& _VertexArray, TUInt32 _Count)
	{
		ARC_PROFILE_FUNCTION();
		_VertexArray->Bind();
		glDrawArrays(GL_LINES, 0, _Count);
	}

	void COpenGLRendererAPI::SetLineThickness(float pThickness)
	{
		glLineWidth(pThickness);
	}
}