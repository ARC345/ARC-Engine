#pragma once
#include <type_traits>
#include "Utils\MPL\Interface.hpp"

namespace ARC {
	template<typename RET>
	class TDelegate;
	template<typename RET>
	class TMulticastDelegate;

	// @TODO rewrite
	template<typename RET, typename ...PARAMS>
	class TDelegate<RET(PARAMS...)> {
	public:
		using Params = MPL::TypeList<PARAMS...>;
		using RVal = RET;

		TDelegate() : fn(nullptr) {}

		template<auto Candidate, typename Type>
		void Bind(Type value_or_instance) {
			static_assert(sizeof(Type) <= sizeof(void*));
			static_assert(std::is_trivially_copyable_v<Type> && std::is_trivially_destructible_v<Type>);
			static_assert(std::is_invocable_r_v<RET, decltype(Candidate), Type&, PARAMS...>);

			new (&storage) Type{ value_or_instance };

			fn = [](storage_type& storage, PARAMS... args) -> RET {
				Type value_or_instance = *reinterpret_cast<Type*>(&storage);
				return std::invoke(Candidate, value_or_instance, args...);
			};
		};
		template<auto Function>
		void Bind() {
			static_assert(std::is_invocable_r_v<RET, decltype(Function), PARAMS...>);

			new (&storage) void* { nullptr };

			fn = [](storage_type&, PARAMS... args) -> RET {
				return std::invoke(Function, args...);
			};
		};
		template<typename Invokable>
		void Bind(Invokable invokable) {
			static_assert(sizeof(Invokable) < sizeof(void*), "capture list not supported currently");
			static_assert(std::is_class_v<Invokable>);
			static_assert(std::is_trivially_destructible_v<Invokable>);
			static_assert(std::is_invocable_r_v<RET, Invokable, PARAMS...>);

			new (&storage) Invokable{ std::move(invokable) };

			fn = [](storage_type& storage, PARAMS... args) -> RET {
				Invokable& invokable = *reinterpret_cast<Invokable*>(&storage);
				return std::invoke(invokable, args...);
			};
		};

		inline
		RET operator()(PARAMS... args) const { return fn(storage, args...); }
		[[nodiscard]] inline explicit 
		operator bool() const { return fn != nullptr; }
		[[nodiscard]] inline 
		bool operator==(const TDelegate<RET(PARAMS...)>& _) const { return fn == _.fn; }
		[[nodiscard]] inline 
		bool operator!=(const TDelegate<RET(PARAMS...)>& _) const { return fn != _.fn; }
	private:
	protected:
		using storage_type = std::aligned_storage_t<sizeof(void*), alignof(void*)>;
		using proto_fn_type = RET(storage_type&, PARAMS...);
		mutable storage_type storage;
		proto_fn_type* fn;
	};
	
	template<typename RET, typename ...PARAMS>
	class TMulticastDelegate<RET(PARAMS...)> {
		using DelegateClass = TDelegate<RET(PARAMS...)>;
		using InvocationListClass = std::list<DelegateClass*>;

	public:
		TMulticastDelegate() = default;
		~TMulticastDelegate() {
			for (auto& element : mInvocationList) delete element;
			mInvocationList.clear();
		};

		[[nodiscard]] inline bool Contains(const DelegateClass& _) const { return (std::find(mInvocationList.begin(), mInvocationList.end(), _)); }
		[[nodiscard]] inline auto Find(const DelegateClass& _) const { return std::find(mInvocationList.begin(), mInvocationList.end(), _); }
		
		[[nodiscard]] inline bool IsEmpty() const { return mInvocationList.size() < 1; }
		[[nodiscard]] inline size_t Size() const { return mInvocationList.size(); }

		void Clear() {
			for (auto& element : mInvocationList)
				delete element;
			mInvocationList.clear();
		}

		template<auto Candidate, typename Type>
		void Bind(Type value_or_instance) {
			auto newelem = new DelegateClass;
			newelem->Bind<Candidate, Type>(value_or_instance);
			mInvocationList.push_back(newelem);
		};
		template<auto Function>
		void Bind() {
			auto newelem = new DelegateClass;
			newelem->Bind<Function>();
			mInvocationList.push_back(newelem);
		};
		template<typename Invokable>
		void Bind(Invokable invokable) {
			auto newelem = new DelegateClass;
			newelem->Bind<Invokable>(invokable);
			mInvocationList.push_back(newelem);
		};

		inline void operator()(PARAMS... args) const { 
			for (auto& element : mInvocationList)
				element->operator ()(args...);
		}
		template<typename HANDLER>
		void operator()(PARAMS... args, HANDLER handler) const {
			size_t index = 0;
			for (auto& item : mInvocationList) {
				RET value = *item(args...);
				handler(index, &value);
				++index;
			} //loop
		}
		[[nodiscard]] inline explicit operator bool() const { return !IsEmpty(); }

		typename InvocationListClass::iterator begin() { return mInvocationList.begin(); }
		typename InvocationListClass::iterator end() { return mInvocationList.end(); }
		typename InvocationListClass::const_iterator begin() const { return mInvocationList.begin(); }
		typename InvocationListClass::const_iterator end() const { return mInvocationList.end(); }

	private:
		InvocationListClass mInvocationList;
	};


}