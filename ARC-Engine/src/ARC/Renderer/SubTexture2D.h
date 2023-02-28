#pragma once
#include "ARC\Types\Pointer.h"
#include "ARC\Types\vector.h"
#include "glm\ext\vector_float2.hpp"

namespace ARC { class CTexture2D; }

namespace ARC {
	class CSubTexture2D
	{
	public:
		CSubTexture2D(const TRef<CTexture2D>& _Texture, const FVec2& _Min, const FVec2& _Max);

		inline const TRef<CTexture2D> GetTexture() const { return mTexture; };
		inline const glm::vec2* GetTexCoords() const { return mTexCoords; };

		static TRef<CSubTexture2D> CreateFromCoords(const TRef<CTexture2D>& _Texture, const FVec2& _Coords, const FVec2& _CellSize, const FVec2& _SpriteSize={1, 1});
	protected:
	private:
		TRef<CTexture2D> mTexture;
		glm::vec2 mTexCoords[4];
	};
}