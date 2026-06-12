#pragma once
#include "TybeBase.h"

namespace arc
{
	namespace Base
	{
		struct array_base : public type_base{};
	}

	template <typename T>
	struct array : public array_base
	{
		/*--------------FUNCS--------------*/
		public:
			array(size_t p_cap = 5u) : cap_(p_cap);
			~array();
			array(const array& p_obj)

			T& operator[](const size_t p_index)

			void init(const T& p_from);
			void init(const size_t& p_from);

			void Expand();
			void Push();

			void Pop(const size_t& p_index);
			void Remove();

			virtual __forceinline const size_t& get_size() const override {
				return len_;
			}
		protected:
		private:
		/*---------------------------------*/
		
		/*---------------VARS--------------*/
		public:
		protected:
		private:
			size_t len_;
			size_t cap_;
			T** arr_;
		/*---------------------------------*/
	};
};