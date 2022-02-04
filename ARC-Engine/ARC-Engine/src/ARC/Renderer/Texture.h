#pragma once
#include "ARC\Interfaces\Area2D.h"

namespace ARC {
	class CTexture
	{
		public:
			virtual ~CTexture() = default;

			virtual void Bind(uint32_t _Slot = 0) const = 0;
	};
	
	class CTexture2D : public CTexture
	{
		public:
			static TRef<CTexture2D> Create(const TVec2<uint32_t> _Dimentions);
			static TRef<CTexture2D> Create(const std::string& _Path);

			virtual void SetData(void*, uint32_t _Size) = 0;
			
		protected:
			inline CTexture2D(TVec2<uint32_t> _Dimentions) : Dimensions(_Dimentions) {}
		public:
			TVec2<uint32_t> Dimensions;
	};
}