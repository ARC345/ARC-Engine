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

		inline static void SetClearColour(const glm::vec4 _Colour) {
			s_RendererAPI->SetClearColour(_Colour);
		};
		inline static void Clear() {
			s_RendererAPI->Clear();
		};
		inline static void DrawIndexed(const TRef<CVertexArray>& _VertexArray) {
			s_RendererAPI->DrawIndexed(_VertexArray);
		}
	private:
		static CRendererAPI* s_RendererAPI;
	};
}