#pragma once
#include <string>
#include "..\Types\vector.h"
#include "glm\ext\vector_float2.hpp"

namespace ARC {
	class CTexture
	{
		public:
			virtual ~CTexture() = default;

			virtual void ClearData() = 0;
			virtual void SetData(void*, uint32_t _Size) = 0;
			virtual void Bind(uint32_t _Slot = 0) const = 0;

			virtual uint32_t GetRendererID() = 0;

			virtual bool operator==(const CTexture& _Tex) const = 0;
	};
	
	class CTexture2D : public CTexture
	{
		public:
			static TRef<CTexture2D> Create(const TVec2<uint32_t> _Dimentions);
			static TRef<CTexture2D> Create(const std::string& _Path, bool bManualClear = false);

			virtual TVec4<unsigned char> GetPixelColor(TVec2<uint32_t> xy) = 0;
			
		protected:
			inline CTexture2D(TVec2<uint32_t> _Dimentions) : Dimensions(_Dimentions) {}
		public:
			TVec2<uint32_t> Dimensions;

			static constexpr glm::vec2 TexCoords[] = {
				{0.f, 0.f},
				{1.f, 0.f},
				{1.f, 1.f},
				{0.f, 1.f}
			};
			
	};
}