#pragma once

#include "RendererAPI.h"
#include "ARC\Types\Pointer.h"

namespace ARC {
	class CRenderCommand
	{
	public:
		inline static void Init() {
			sRendererAPI->Init();
		};

		inline static void SetViewport(const TVec2<uint32_t> _BottemLeftCoord, const TVec2<uint32_t> _Dimentions) {
			sRendererAPI->SetViewport(_BottemLeftCoord, _Dimentions);
		}
		inline static void SetClearColour(const FColor4 _Colour) {
			sRendererAPI->SetClearColour(_Colour);
		};
		inline static void Clear() {
			sRendererAPI->Clear();
		};

		// Draw All if _Count = 0
		inline static void DrawIndexed(const TRef<CVertexArray>& _VertexArray, uint32_t _Count = 0) {
			sRendererAPI->DrawIndexed(_VertexArray, _Count);
		}
		inline static void DrawLine(const TRef<CVertexArray>& _VertexArray, uint32_t _Count = 0) {
			sRendererAPI->DrawLine(_VertexArray, _Count);
		}
		inline static void SetLineThickness(float pThickness) {
			sRendererAPI->SetLineThickness(pThickness);
		}

	private:
		static CRendererAPI* sRendererAPI;
	};
}