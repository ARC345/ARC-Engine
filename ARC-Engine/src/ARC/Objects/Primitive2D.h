#pragma once
#include "Object.h"
#include "ARC/Types/Color.h"
#include "ARC\Types\Vector.h"
#include "ARC\Types\Transform2D.h"
#include "ARC\Types\Pointer.h"

namespace ARC { class CSubTexture2D; }
namespace ARC { class CTexture2D; }

namespace ARC {
	class CPrimitive2D :
		public CObject
	{
	public:
		inline const FVec3& GetLocation() const { return Transform.Location; }
		inline FVec3 GetLocation() { return Transform.Location; }

		inline const float& GetRotation() const { return Transform.Rotation; }
		inline float GetRotation() { return Transform.Rotation; }

		inline const FVec2& GetScale() const { return Transform.Scale; }
		inline FVec2 GetScale() { return Transform.Scale; }

		inline const FTransform2D& GetTransform() const { return Transform; }
		inline FTransform2D GetTransform() { return Transform; }

	public:
		FTransform2D Transform;
		FColor Color = FColor::White;
		TRef<CTexture2D> Texture = nullptr;
		FVec2 TextureScaling = {1, 1};
	};
}