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

		static FGLMMat4 GetViewProjectionMatrix() { return mSceneData->ViewProjectionMatrix; }

		static void Submit(const TRef<CShader>& _Shader, const TRef<CVertexArray>& _VertexArray, const FGLMMat4& _Transform = FGLMMat4(1.f));

		inline static CRendererAPI::ERendererAPI GetCurrentAPI() { return CRendererAPI::GetAPI(); }

	private:
		struct SSceneData
		{
			FGLMMat4 ViewProjectionMatrix;
		};

		static SSceneData* mSceneData;
	};	
}