#include "arc_pch.h"
#include "SubTexture2D.h"
#include "ARC\Renderer\Texture.h"

namespace ARC {

	CSubTexture2D::CSubTexture2D(const TRef<CTexture2D>& _Texture, const FVec2& _Min, const FVec2& _Max) :
		m_Texture(_Texture)
	{
		m_TexCoords[0] = { _Min.x, _Min.y };
		m_TexCoords[1] = { _Max.x, _Min.y };
		m_TexCoords[2] = { _Max.x, _Max.y };
		m_TexCoords[3] = { _Min.x, _Max.y };
	}

	TRef<CSubTexture2D> CSubTexture2D::CreateFromCoords(const TRef<CTexture2D>& _Texture, const FVec2& _Coords, const FVec2& _CellSize, const FVec2& _SpriteSize)
	{
		return CreateRef<CSubTexture2D>(
			_Texture,
			FVec2(_Coords.x * _CellSize.x / _Texture->Dimensions.x, _Coords.y * _CellSize.y / _Texture->Dimensions.y),
			FVec2((_Coords.x + _SpriteSize.x) * _CellSize.x / _Texture->Dimensions.x, (_Coords.y + _SpriteSize.y) * _CellSize.y / _Texture->Dimensions.y)
			);
	}

}