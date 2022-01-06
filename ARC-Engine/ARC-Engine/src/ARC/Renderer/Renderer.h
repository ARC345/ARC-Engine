#pragma once
#include "RenderCommand.h"
#include "RendererAPI.h"
#include "../../../out/glm/glm/glm.hpp"

namespace ARC { class CShader; }
namespace ARC { class CVertexArray; }
namespace ARC { class COrthographicCameraBase; }

namespace ARC {
	class CRenderer {
	public:
		static void BeginScene(COrthographicCameraBase& _Cam);
		static void EndScene();

		static void Submit(const std::shared_ptr<CShader>& _Shader, const std::shared_ptr<CVertexArray>& _VertexArray);

		inline static CRendererAPI::ERendererAPI GetCurrentAPI() { return CRendererAPI::GetAPI(); }

	private:
		struct SSceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SSceneData* m_SceneData;
	};
}