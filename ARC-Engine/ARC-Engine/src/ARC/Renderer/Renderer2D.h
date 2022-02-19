#pragma once
#include "Color.h"

namespace ARC { class CPrimitive2D; }

namespace ARC { class COrthographicCameraBase; }
namespace ARC { class CVertexArray; }
namespace ARC { class CTexture2D; }
namespace ARC { class CShader; }

namespace ARC {
	class CRenderer2D
	{
		public:
			static void Init();
			static void Shutdown();

			static void BeginScene(const COrthographicCameraBase& _Camera);
			static void EndScene();
			
			static void Flush();

			// Primitives
			static void DrawQuad(const FVec2 _Position, const float _Rotation = 0.f, const FVec2 _Size = {1.f, 1.f}, float _ZOrder = 0, const CColor _Color = CColor::Red);
			static void DrawQuad(const FVec2 _Position, const float _Rotation, const FVec2 _Size, float _ZOrder, const FVec2 _TextureScaling, const CColor _Color, const TRef<CTexture2D> _Tex);
			static void DrawQuad(CPrimitive2D& Quad);
	};
}