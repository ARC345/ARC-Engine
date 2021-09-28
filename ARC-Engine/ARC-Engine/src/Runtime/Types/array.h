#pragma once
#include "container.h"

namespace ARC
{
	namespace Base
	{
		struct FArray_Base : public FContainer{};
	}

	template <typename T = float>
	struct FArray : public FArray_Base
	{
		/*--------------FUNCS--------------*/
		public:
		protected:

			constexpr size_t GetSize() const override {
				return m_Len;
			}

		private:
		/*---------------------------------*/
		
		/*---------------VARS--------------*/
		public:
		protected:
		private:
			size_t m_Len;
		/*---------------------------------*/
	};
};