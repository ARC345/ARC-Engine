#pragma once
#include "ARC\Interfaces\Area2D.h"

namespace ARC {
	class CTexture
	{
		public:
			virtual ~CTexture() = default;

			virtual void Bind(uint32_t _Slot = 0) const = 0;
	};
	
	class CTexture2D : public CTexture, public IArea2D<uint32_t>
	{
		public:
			static TRef<CTexture2D> Create(const std::string& _Path);
		protected:

			inline CTexture2D(uint32_t _X, uint32_t _Y) : IArea2D<uint32_t>(_X, _Y) {}
			inline CTexture2D(std::pair<uint32_t, uint32_t> _Dimentions) : IArea2D<uint32_t>(_Dimentions.first, _Dimentions.second) {}
	};
}