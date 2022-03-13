#pragma once
#include "TybeBase.h"

namespace ARC
{
	namespace Base
	{
		struct FArray_Base : public TTypeBase{};
	}

	template <typename T>
	struct TArray : public FArray_Base
	{
		/*--------------FUNCS--------------*/
		public:
			TArray(size_t p_cap = 5u) : m_Cap(p_cap);
			~TArray();
			TArray(const TArray& p_obj)

			T& operator[](const size_t p_index)

			void Init(const T& p_from);
			void Init(const size_t& p_from);

			void Expand();
			void Push();

			void Pop(const size_t& p_index);
			void Remove();

			virtual __forceinline const size_t& GetSize() const override {
				return m_Len;
			}
		protected:
		private:
		/*---------------------------------*/
		
		/*---------------VARS--------------*/
		public:
		protected:
		private:
			size_t m_Len;
			size_t m_Cap;
			T** m_Arr;
		/*---------------------------------*/
	};
};