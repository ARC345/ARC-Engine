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
			/*
			*	@param _Position: Center location of quad.
			*	@param _Rotation: Rotation of quad in radians.
			*	@param _Size: Size of quad.
			*	@param _Zorder: Z position of quad.
			*	@param _Color: Color of quad.
			*/
			static void DrawQuad(const FVec2& _Position, const float _Rotation = 0.f, const FVec2& _Size = {1.f, 1.f}, float _ZOrder = 0, const CColor& _Color = CColor::Red);

			/*
			*	@param _Position: Center location of quad.
			*	@param _Rotation: Rotation of quad in radians.
			*	@param _Size: Size of quad.
			*	@param _Zorder: Z position of quad.
			*	@param _TextureScaling: Scaling applied to the texture
			*	@param _Color: Color of quad.
			*	@param _Texture: Texture of quad.
			*/
			static void DrawQuad(const FVec2& _Position, const float _Rotation, const FVec2& _Size, float _ZOrder, const FVec2& _TextureScaling, const CColor& _Color, const TRef<CTexture2D>& _Tex);
			static void DrawQuad(CPrimitive2D& Quad);

			struct SStatistics
			{
				uint32_t DrawCalls = 0;
				uint32_t QuadCount = 0;

				uint32_t GetTotalVertexCount() { return QuadCount * 4; }
				uint32_t GetTotalIndexCount() { return QuadCount * 6; }
			};
			static SStatistics GetStats();
			static void ResetStats();

		private:
			static void FlushAndReset();
	};
}