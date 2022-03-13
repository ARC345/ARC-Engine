#pragma once
#include "ARC\Renderer\RendererAPI.h"

namespace ARC {
	class COpenGLRendererAPI : public CRendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(const TVec2<uint32_t> _BottemLeftCoord, const TVec2<uint32_t> _Dimentions) override;
		virtual void SetClearColour(const glm::vec4 _Colour) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const TRef<CVertexArray>& _VertexArray, uint32_t _Count) override;
	};
}