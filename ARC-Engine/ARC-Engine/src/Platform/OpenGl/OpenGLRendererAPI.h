#pragma once
#include "ARC\Renderer\RendererAPI.h"

namespace ARC {
	class COpenGLRendererAPI : public CRendererAPI
	{
	public:
		virtual void SetClearColour(const glm::vec4 _Colour) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const std::shared_ptr<CVertexArray>& _VertexArray) override;

	};
}