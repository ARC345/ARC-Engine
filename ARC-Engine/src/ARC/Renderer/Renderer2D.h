#pragma once
#include "ARC/Types/Color.h"
#include "ARC/Types/Transform2D.h"

namespace ARC { class CEditorCamera; }

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

			static void BeginScene(const COrthographicCamera& pCamera);
			static void BeginScene(const CEditorCamera& pCamera);
			static void BeginScene(const CCamera& pCamera, const FTransform2D& pTransform);

			static void EndScene_Translucent();
			static void EndScene_Opaque();
			static void EndScene();
			
			static void FlushTranslucent();
			static void FlushOpaque();

			/*
			*	@param pPosition: Center location of quad.
			*	@param pRotation: Rotation of quad in radians.
			*	@param pSize: Size of quad.
			*	@param pColor: Color of quad.
			*	@param pTexture: Texture of quad.
			*	@param pTextureScaling: Scaling applied to the texture.
			*/
			static void DrawQuad(const FVec3& pPosition, const float pRotation = 0.f, const FVec2& pSize = FVec2::OneVector(), const ETransparencyType pTransparencyLevel = ETransparencyType::Translucent, const FColor4& pColor = FColor4::White(), const TRef<CTexture2D>& pTex = nullptr, const FVec2& pTextureScaling = FVec2::OneVector(), const int& pId = -1);
			/*
			*	@param pPosition: Center location of quad.
			*	@param pRotation: Rotation of quad in radians.
			*	@param pSize: Size of quad.
			*	@param pColor: Color of quad.
			*	@param pTexture: Texture of quad.
			*	@param pTextureScaling: Scaling applied to the texture.
			*/
			static void DrawQuad(const FVec3& pPosition, const float pRotation, const FVec2& pSize, const ETransparencyType pTransparencyLevel, const FColor4& pColor, const TRef<CSubTexture2D>& pTex, const FVec2& pTextureScaling, const int& pId = -1);
			static void DrawQuad(const CPrimitive2D& Quad, const int& pId = -1);

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
			static void FlushAndReset_Translucent();
			static void FlushAndReset_Opaque();
			static void FlushAndReset();
	};
}
