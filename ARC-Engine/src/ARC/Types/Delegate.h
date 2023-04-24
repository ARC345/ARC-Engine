#pragma once
#include <type_traits>
#include <list>
#include <functional>

namespace ARC {
	template<typename RET>
	class TDelegateBase;
	template<typename RET>
	class TDelegate;
	template<typename RET>
	class TMulticastDelegate;

	template<typename RET, typename ...PARAMS>
	class TDelegateBase<RET(PARAMS...)> {
	protected:
		using stub_type = RET(*)(void* this_ptr, PARAMS...);

		struct InvocationElement {
			InvocationElement() = default;
			InvocationElement(void* this_ptr, stub_type aStub) : Object(this_ptr), Stub(aStub) {}
			void Clone(InvocationElement& target) const {
				target.Stub = Stub;
				target.Object = Object;
			}
			bool operator ==(const InvocationElement& another) const {
				return another.Stub == Stub && another.Object == Object;
			}
			bool operator !=(const InvocationElement& another) const {
				return another.Stub != Stub || another.Object != Object;
			}
			void* Object = nullptr;
			stub_type Stub = nullptr;
		}; 

	};

	template<typename RET, typename ...PARAMS>
	class TDelegate<RET(PARAMS...)> final : private TDelegateBase<RET(PARAMS...)> {
	public:

		TDelegate() = default;

		operator bool() const { return invocation.Stub == nullptr; }
		bool operator ==(void* ptr) const {
			return (ptr == nullptr) && this->IsNull();
		} //operator ==
		bool operator !=(void* ptr) const {
			return (ptr != nullptr) || (!this->IsNull());
		} //operator !=

		TDelegate(const TDelegate& another) { another.invocation.Clone(invocation); }

		template <typename LAMBDA>
		TDelegate(const LAMBDA& lambda) {
			Bind((void*)(&lambda), LambdaStub<LAMBDA>);
		} //TDelegate

		TDelegate& operator =(const TDelegate& another) {
			another.invocation.Clone(invocation);
			return *this;
		} //operator =

		template <typename LAMBDA> // template instantiation is not needed, will be deduced (inferred):
		TDelegate& operator =(const LAMBDA& instance) {
			Bind((void*)(&instance), LambdaStub<LAMBDA>);
			return *this;
		} //operator =

		bool operator == (const TDelegate& another) const { return invocation == another.invocation; }
		bool operator != (const TDelegate& another) const { return invocation != another.invocation; }

		bool operator ==(const TMulticastDelegate<RET(PARAMS...)>& another) const { return another == (*this); }
		bool operator !=(const TMulticastDelegate<RET(PARAMS...)>& another) const { return another != (*this); }

		template <class T, RET(T::* TMethod)(PARAMS...)>
		static TDelegate Create(T* instance) {
			return TDelegate(instance, MethodStub<T, TMethod>);
		}

		template <class T, RET(T::* TMethod)(PARAMS...) const>
		static TDelegate Create(T const* instance) {
			return TDelegate(const_cast<T*>(instance), ConstMethodStub<T, TMethod>);
		}

		template <RET(*TMethod)(PARAMS...)>
		static TDelegate Create() {
			return TDelegate(nullptr, FunctionStub<TMethod>);
		}

		template <typename LAMBDA>
		static TDelegate Create(const LAMBDA& instance) {
			return TDelegate((void*)(&instance), LambdaStub<LAMBDA>);
		} 

		RET operator()(PARAMS... arg) const {
			return (*invocation.Stub)(invocation.Object, arg...);
		}

	private:

		TDelegate(void* anObject, typename TDelegateBase<RET(PARAMS...)>::stub_type aStub) {
			invocation.Object = anObject;
			invocation.Stub = aStub;
		} 

		void Bind(void* anObject, typename TDelegateBase<RET(PARAMS...)>::stub_type aStub) {
			this->invocation.Object = anObject;
			this->invocation.Stub = aStub;
		} 

		template <class T, RET(T::* TMethod)(PARAMS...)>
		static RET MethodStub(void* this_ptr, PARAMS... params) {
			T* p = static_cast<T*>(this_ptr);
			return (p->*TMethod)(params...);
		} 

		template <class T, RET(T::* TMethod)(PARAMS...) const>
		static RET ConstMethodStub(void* this_ptr, PARAMS... params) {
			T* const p = static_cast<T*>(this_ptr);
			return (p->*TMethod)(params...);
		} 

		template <RET(*TMethod)(PARAMS...)>
		static RET FunctionStub(void* this_ptr, PARAMS... params) {
			return (TMethod)(params...);
		} 

		template <typename LAMBDA>
		static RET LambdaStub(void* this_ptr, PARAMS... arg) {
			LAMBDA* p = static_cast<LAMBDA*>(this_ptr);
			return (p->operator())(arg...);
		} 

		friend class TMulticastDelegate<RET(PARAMS...)>;
		typename TDelegateBase<RET(PARAMS...)>::InvocationElement invocation;

	}; 
	template<typename RET, typename ...PARAMS>
	class TMulticastDelegate<RET(PARAMS...)> final : private TDelegateBase<RET(PARAMS...)> {
	public:

		TMulticastDelegate() = default;
		~TMulticastDelegate() {
			for (auto& element : invocationList) delete element;
			invocationList.clear();
		} //~TMulticastDelegate

		operator bool() const { return invocationList.size() < 1; }
		bool operator ==(void* ptr) const {
			return (ptr == nullptr) && *this;
		} //operator ==
		bool operator !=(void* ptr) const {
			return (ptr != nullptr) || (!*this);
		} //operator !=

		size_t size() const { return invocationList.size(); }

		TMulticastDelegate& operator =(const TMulticastDelegate&) = delete;
		TMulticastDelegate(const TMulticastDelegate&) = delete;

		bool operator ==(const TMulticastDelegate& another) const {
			if (invocationList.size() != another.invocationList.size()) return false;
			auto anotherIt = another.invocationList.begin();
			for (auto it = invocationList.begin(); it != invocationList.end(); ++it)
				if (**it != **anotherIt) return false;
			return true;
		} //==
		bool operator !=(const TMulticastDelegate& another) const { return !(*this == another); }

		bool operator ==(const TDelegate<RET(PARAMS...)>& another) const {
			if (*this && another) return true;
			if (another || (size() != 1)) return false;
			return (another.invocation == **invocationList.begin());
		} //==
		bool operator !=(const TDelegate<RET(PARAMS...)>& another) const { return !(*this == another); }

		TMulticastDelegate& operator +=(const TMulticastDelegate& another) {
			for (auto& item : another.invocationList) // clone, not copy; flattens hierarchy:
				this->invocationList.push_back(new typename TDelegateBase<RET(PARAMS...)>::InvocationElement(item->Object, item->Stub));
			return *this;
		} //operator +=

		template <typename LAMBDA> // template instantiation is not neededm, will be deduced/inferred:
		TMulticastDelegate& operator +=(const LAMBDA& lambda) {
			TDelegate<RET(PARAMS...)> d = TDelegate<RET(PARAMS...)>::template create<LAMBDA>(lambda);
			return *this += d;
		} //operator +=

		TMulticastDelegate& operator +=(const TDelegate<RET(PARAMS...)>& another) {
			if (another) return *this;
			this->invocationList.push_back(new typename TDelegateBase<RET(PARAMS...)>::InvocationElement(another.invocation.Object, another.invocation.Stub));
			return *this;
		} //operator +=

		// will work even if RET is void, return values are ignored:
		// (for handling return values, see operator(..., handler))
		void operator()(PARAMS... arg) const {
			for (auto& item : invocationList)
				(*(item->Stub))(item->Object, arg...);
		} //operator()

		template<typename HANDLER>
		void operator()(PARAMS... arg, HANDLER handler) const {
			size_t index = 0;
			for (auto& item : invocationList) {
				RET value = (*(item->Stub))(item->Object, arg...);
				handler(index, &value);
				++index;
			} //loop
		} //operator()

		void operator()(PARAMS... arg, TDelegate<void(size_t, RET*)> handler) const {
			operator() < decltype(handler) > (arg..., handler);
		} //operator()
		void operator()(PARAMS... arg, std::function<void(size_t, RET*)> handler) const {
			operator() < decltype(handler) > (arg..., handler);
		} //operator()

	private:

		std::list<typename TDelegateBase<RET(PARAMS...)>::InvocationElement*> invocationList;

	}; //class TMulticastDelegate
}