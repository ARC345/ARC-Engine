#pragma once
#include "ARC/Types/Color.h"
#include "ARC/Types/Transform2D.h"
#include "../Scene/Entity.h"

namespace ARC { class CEditorCamera; }

namespace ARC { class CEditorCamera; }

namespace ARC { class CCamera; }

namespace ARC { class CSubTexture2D; }
namespace ARC { class CPrimitive2D; }

namespace ARC { class COrthographicCamera; }
namespace ARC { class CVertexArray; }
namespace ARC { class CTexture2D; }
namespace ARC { class CShader; }

namespace ARC {
	
	namespace EGeometery {
		enum EGeometery { Quad = 1, Circle= 2, Line=3};
	};
	using TGeometery = TUInt8;
	class SRenderer2D
	{
		public:
			static void Init();
			static void Shutdown();

			static void BeginScene(const COrthographicCamera& pCamera);
			static void BeginScene(const CEditorCamera& pCamera);
			static void BeginScene(const CCamera& pCamera, const FTransform2D& pTransform);

			constexpr static void EndScene(TGeometery pG = EGeometery::Quad | EGeometery::Circle | EGeometery::Line, TTransparencyType pT = ETransparencyType::Opaque | ETransparencyType::Translucent | ETransparencyType::Transparent);
			constexpr static void Flush(TGeometery pG = EGeometery::Quad | EGeometery::Circle | EGeometery::Line, TTransparencyType pT = ETransparencyType::Opaque | ETransparencyType::Translucent | ETransparencyType::Transparent);
			/*
			*	@param pPosition: Center location of quad.
			*	@param pRotation: Rotation of quad in radians.
			*	@param pSize: Size of quad.
			*	@param pColor: Color of quad.
			*	@param pTexture: Texture of quad.
			*	@param pTextureScaling: Scaling applied to the texture.
			*/
			static void DrawQuad(const FVec3& pPosition, const float pRotation = 0.f, const FVec2& pSize = FVec2::OneVector(), const TTransparencyType pTransparencyLevel = ETransparencyType::Translucent, const FColor4& pColor = FColor4::White(), const TRef<CTexture2D>& pTex = nullptr, const FVec2& pTextureScaling = FVec2::OneVector(), const int& pId = -1);
			/*
			*	@param pPosition: Center location of quad.
			*	@param pRotation: Rotation of quad in radians.
			*	@param pSize: Size of quad.
			*	@param pColor: Color of quad.
			*	@param pTexture: Texture of quad.
			*	@param pTextureScaling: Scaling applied to the texture.
			*/
			static void DrawQuad(const FVec3& pPosition, const float pRotation, const FVec2& pSize, const TTransparencyType pTransparencyLevel, const FColor4& pColor, const TRef<CSubTexture2D>& pTex, const FVec2& pTextureScaling, const int& pId = -1);
			static void DrawQuad(const CPrimitive2D& Quad, const int& pId = -1);

			static void DrawCircle(const FVec3& pPosition, const float pRotation = 0.f, const FVec2& pSize = FVec2::OneVector(), const FColor4& pColor = FColor4::White(), const float pThickness = 1.f, const float pSharpness = 0.995f, const int& pId = -1);
			static void DrawLine(const FVec3& pPositionStart, const FVec3& pPositionEnd, const FColor4& pColor = FColor4::White(), const int& pId = -1);
			static void DrawRect(const FVec3& pPosition, const float pRotation, const FVec2& pSize, const FColor4& pColor, const int& pId = -1);
			
			static void SetLineThickness(float pThickness);
			static float GetLineThickness();

			struct SStatistics
			{
				TUInt32 DrawCalls = 0;
				TUInt32 QuadCount = 0;
				
				TUInt32 GetTotalVertexCount() const { return QuadCount * 4; }
				TUInt32 GetTotalIndexCount() const{ return QuadCount * 6; }
			};
			static SStatistics GetStats();
			static void ResetStats();

		private:
			constexpr static void FlushAndReset(TGeometery pG, TTransparencyType pT);
	};
}
