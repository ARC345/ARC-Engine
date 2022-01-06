#pragma once

#include "..\..\..\out\glm\glm\glm.hpp"

namespace ARC { class CVertexArray; }

namespace ARC {
	class CRendererAPI {
	public:
		enum class ERendererAPI
		{
			None = 0,
			OpenGL = 1
		};
	public:
		virtual void SetClearColour(const glm::vec4 _Colour) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<CVertexArray>& _VertexArray) = 0;
	
		inline static ERendererAPI GetAPI() { return s_API; };
	private:
		static ERendererAPI s_API;
	};
}