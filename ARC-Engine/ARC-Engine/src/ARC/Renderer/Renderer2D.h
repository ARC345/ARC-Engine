#pragma once
#include "Color.h"

namespace ARC { class COrthographicCameraBase; }
namespace ARC { class CVertexArray; }
namespace ARC { class CTexture2D; }
namespace ARC { class CShader; }

namespace ARC {
	struct SRenderer2DStorage {
		TRef<CVertexArray> QuadVertexArray;
		TRef<CTexture2D> WhiteTexture;
		TRef<CShader> TextureShader;
	};
	class CRenderer2D
	{
		public:
			static void Init();
			static void Shutdown();

			static void BeginScene(const COrthographicCameraBase& _Camera);
			static void EndScene();

			// Primitives
			static void DrawQuad(const TVec2<> _Position, const float _Rotation = 0.f, const TVec2<> _Size = {1.f, 1.f}, int _ZOrder = 0, CColor _Color = CColor::Red);
			static void DrawQuad(const TVec2<> _Position, const float _Rotation, const TVec2<> _Size, int _ZOrder, CColor _Color, TRef<CTexture2D> _Tex);
	};
}