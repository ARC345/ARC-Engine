#pragma once
#include "ARC/Types/Color.h"
#include "ARC/Types/Transform2D.h"

namespace ARC { class CCamera; }

namespace ARC { class CSubTexture2D; }
namespace ARC { class CPrimitive2D; }

namespace ARC { class COrthographicCamera; }
namespace ARC { class CVertexArray; }
namespace ARC { class CTexture2D; }
namespace ARC { class CShader; }

namespace ARC {
	class CRenderer2D
	{
		public:
			static void Init();
			static void Shutdown();

			static void BeginScene(const COrthographicCamera& _Camera);
			static void BeginScene(const CCamera& _Camera, const FTransform2D& _Transform);

			static void EndScene();
			
			static void Flush();

			/*
			*	@param _Position: Center location of quad.
			*	@param _Rotation: Rotation of quad in radians.
			*	@param _Size: Size of quad.
			*	@param _Zorder: Z position of quad.
			*	@param _Color: Color of quad.
			*	@param _Texture: Texture of quad.
			*	@param _TextureScaling: Scaling applied to the texture.
			*/
			static void DrawQuad(const FVec3& _Position, const float _Rotation = 0.f, const FVec2& _Size = { 1.f, 1.f }, const FColor& _Color = FColor::White, const TRef<CTexture2D>& _Tex = nullptr, const FVec2& _TextureScaling = {1.f, 1.f});
			/*
			*	@param _Position: Center location of quad.
			*	@param _Rotation: Rotation of quad in radians.
			*	@param _Size: Size of quad.
			*	@param _Zorder: Z position of quad.
			*	@param _Color: Color of quad.
			*	@param _Texture: Texture of quad.
			*	@param _TextureScaling: Scaling applied to the texture.
			*/
			static void DrawQuad(const FVec3& _Position, const float _Rotation, const FVec2& _Size, const FColor& _Color, const TRef<CSubTexture2D>& _Tex, const FVec2& _TextureScaling);
			static void DrawQuad(const CPrimitive2D& Quad);

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