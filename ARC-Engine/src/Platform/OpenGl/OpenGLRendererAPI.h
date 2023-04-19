#pragma once
#include "ARC\Renderer\RendererAPI.h"

namespace ARC {
	class COpenGLRendererAPI : public CRendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(const TVec2<uint32_t> _BottemLeftCoord, const TVec2<uint32_t> _Dimentions) override;
		virtual void SetClearColour(const FColor4 _Colour) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const TRef<CVertexArray>& _VertexArray, uint32_t _Count) override;
		virtual void DrawLine(const TRef<CVertexArray>& _VertexArray, uint32_t _Count) override;
		virtual void SetLineThickness(float pThickness) override;
	};
}