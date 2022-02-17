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
		static void Init();
		static void OnWindowResize(TVec2<uint32_t> _Dimentions);
		static void BeginScene(COrthographicCameraBase& _Cam);
		static void EndScene();

		static glm::mat4 GetViewProjectionMatrix() { return m_SceneData->ViewProjectionMatrix; }

		static void Submit(const TRef<CShader>& _Shader, const TRef<CVertexArray>& _VertexArray, const glm::mat4& _Transform = glm::mat4(1.f));

		inline static CRendererAPI::ERendererAPI GetCurrentAPI() { return CRendererAPI::GetAPI(); }

	private:
		struct SSceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SSceneData* m_SceneData;
	};

	
}