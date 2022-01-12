#pragma once
#include "ARC\Renderer\RendererAPI.h"

namespace ARC {
	class COpenGLRendererAPI : public CRendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetClearColour(const glm::vec4 _Colour) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const TRef<CVertexArray>& _VertexArray) override;

	};
}