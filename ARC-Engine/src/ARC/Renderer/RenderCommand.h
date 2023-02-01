#pragma once

#include "RendererAPI.h"
#include "ARC\Types\Pointer.h"

namespace ARC {
	class CRenderCommand
	{
	public:
		inline static void Init() {
			s_RendererAPI->Init();
		};

		inline static void SetViewport(const TVec2<uint32_t> _BottemLeftCoord, const TVec2<uint32_t> _Dimentions) {
			s_RendererAPI->SetViewport(_BottemLeftCoord, _Dimentions);
		}
		inline static void SetClearColour(const FColor4 _Colour) {
			s_RendererAPI->SetClearColour(_Colour);
		};
		inline static void Clear() {
			s_RendererAPI->Clear();
		};

		// Draw All if _Count = 0
		inline static void DrawIndexed(const TRef<CVertexArray>& _VertexArray, uint32_t _Count = 0) {
			s_RendererAPI->DrawIndexed(_VertexArray, _Count);
		}
	private:
		static CRendererAPI* s_RendererAPI;
	};
}