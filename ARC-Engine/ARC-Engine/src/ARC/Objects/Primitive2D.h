#pragma once
#include "Object.h"
#include "ARC\Renderer\Color.h"
#include "ARC\Types\Vector.h"
#include "ARC\Types\Transform2D.h"
#include "ARC\Types\Pointer.h"

namespace ARC { class CTexture2D; }

namespace ARC {
	class CPrimitive2D :
		public CObject
	{
	public:
		inline const FVec2& GetLocation() const { return Transform.Location; }
		inline FVec2 GetLocation() { return Transform.Location; }

		inline const float& GetRotation() const { return Transform.Rotation; }
		inline float GetRotation() { return Transform.Rotation; }

		inline const FVec2& GetScale() const { return Transform.Scale; }
		inline FVec2 GetScale() { return Transform.Scale; }

		inline const TTransform2D<>& GetTransform() const { return Transform; }
		inline TTransform2D<> GetTransform() { return Transform; }

	public:
		FTransform2D Transform;
		CColor Color = CColor::White;
		TRef<CTexture2D> Texture = nullptr;
		FVec2 TilingFactor = {1, 1};
	};
}