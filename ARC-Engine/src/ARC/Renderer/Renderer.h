#pragma once
#include "RenderCommand.h"
#include "RendererAPI.h"

#include "ARC\Wrappers\Glm.h"

namespace ARC { class CShader; }
namespace ARC { class CVertexArray; }
namespace ARC { class COrthographicCamera; }

namespace ARC {
	class CRenderer {
	public:
		static void Init();
		static void OnWindowResize(TVec2<uint32_t> _Dimentions);
		static void BeginScene(COrthographicCamera& _Cam);
		static void EndScene();

		static FGMat4 GetViewProjectionMatrix() { return m_SceneData->ViewProjectionMatrix; }

		static void Submit(const TRef<CShader>& _Shader, const TRef<CVertexArray>& _VertexArray, const FGMat4& _Transform = FGMat4(1.f));

		inline static CRendererAPI::ERendererAPI GetCurrentAPI() { return CRendererAPI::GetAPI(); }

	private:
		struct SSceneData
		{
			FGMat4 ViewProjectionMatrix;
		};

		static SSceneData* m_SceneData;
	};	
}