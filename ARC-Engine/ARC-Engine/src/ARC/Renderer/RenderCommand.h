#pragma once

#include "RendererAPI.h"

namespace ARC {
	class CRenderCommand
	{
	public:

		inline static void SetClearColour(const glm::vec4 _Colour) {
			s_RendererAPI->SetClearColour(_Colour);
		};
		inline static void Clear() {
			s_RendererAPI->Clear();
		};
		inline static void DrawIndexed(const std::shared_ptr<CVertexArray>& _VertexArray) {
			s_RendererAPI->DrawIndexed(_VertexArray);
		}
	private:
		static CRendererAPI* s_RendererAPI;
	};
}