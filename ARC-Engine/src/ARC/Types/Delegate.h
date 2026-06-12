#pragma once
#include <type_traits>
#include <list>
#include <functional>

namespace arc {
	template<typename RET>
	class delegate_base;
	template<typename RET>
	class delegate;
	template<typename RET>
	class multicast_delegate;

	template<typename RET, typename ...PARAMS>
	class delegate_base<RET(PARAMS...)> {
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
	class delegate<RET(PARAMS...)> final : private delegate_base<RET(PARAMS...)> {
	public:

		delegate() = default;

		operator bool() const { return invocation.Stub == nullptr; }
		bool operator ==(void* ptr) const {
			return (ptr == nullptr) && this->is_null();
		} //operator ==
		bool operator !=(void* ptr) const {
			return (ptr != nullptr) || (!this->is_null());
		} //operator !=

		delegate(const delegate& another) { another.invocation.Clone(invocation); }

		template <typename LAMBDA>
		delegate(const LAMBDA& lambda) {
			bind((void*)(&lambda), LambdaStub<LAMBDA>);
		} //delegate

		delegate& operator =(const delegate& another) {
			another.invocation.Clone(invocation);
			return *this;
		} //operator =

		template <typename LAMBDA> // template instantiation is not needed, will be deduced (inferred):
		delegate& operator =(const LAMBDA& instance) {
			bind((void*)(&instance), LambdaStub<LAMBDA>);
			return *this;
		} //operator =

		bool operator == (const delegate& another) const { return invocation == another.invocation; }
		bool operator != (const delegate& another) const { return invocation != another.invocation; }

		bool operator ==(const multicast_delegate<RET(PARAMS...)>& another) const { return another == (*this); }
		bool operator !=(const multicast_delegate<RET(PARAMS...)>& another) const { return another != (*this); }

		template <class T, RET(T::* TMethod)(PARAMS...)>
		static delegate create(T* instance) {
			return delegate(instance, MethodStub<T, TMethod>);
		}

		template <class T, RET(T::* TMethod)(PARAMS...) const>
		static delegate create(T const* instance) {
			return delegate(const_cast<T*>(instance), ConstMethodStub<T, TMethod>);
		}

		template <RET(*TMethod)(PARAMS...)>
		static delegate create() {
			return delegate(nullptr, FunctionStub<TMethod>);
		}

		template <typename LAMBDA>
		static delegate create(const LAMBDA& instance) {
			return delegate((void*)(&instance), LambdaStub<LAMBDA>);
		} 

		RET operator()(PARAMS... arg) const {
			return (*invocation.Stub)(invocation.Object, arg...);
		}

	private:

		delegate(void* anObject, typename delegate_base<RET(PARAMS...)>::stub_type aStub) {
			invocation.Object = anObject;
			invocation.Stub = aStub;
		} 

		void bind(void* anObject, typename delegate_base<RET(PARAMS...)>::stub_type aStub) {
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

		friend class multicast_delegate<RET(PARAMS...)>;
		typename delegate_base<RET(PARAMS...)>::InvocationElement invocation;

	}; 
	template<typename RET, typename ...PARAMS>
	class multicast_delegate<RET(PARAMS...)> final : private delegate_base<RET(PARAMS...)> {
	public:

		multicast_delegate() = default;
		~multicast_delegate() {
			for (auto& element : invocationList) delete element;
			invocationList.clear();
		} //~multicast_delegate

		operator bool() const { return invocationList.size() < 1; }
		bool operator ==(void* ptr) const {
			return (ptr == nullptr) && *this;
		} //operator ==
		bool operator !=(void* ptr) const {
			return (ptr != nullptr) || (!*this);
		} //operator !=

		size_t size() const { return invocationList.size(); }

		multicast_delegate& operator =(const multicast_delegate&) = delete;
		multicast_delegate(const multicast_delegate&) = delete;

		bool operator ==(const multicast_delegate& another) const {
			if (invocationList.size() != another.invocationList.size()) return false;
			auto anotherIt = another.invocationList.begin();
			for (auto it = invocationList.begin(); it != invocationList.end(); ++it)
				if (**it != **anotherIt) return false;
			return true;
		} //==
		bool operator !=(const multicast_delegate& another) const { return !(*this == another); }

		bool operator ==(const delegate<RET(PARAMS...)>& another) const {
			if (*this && another) return true;
			if (another || (size() != 1)) return false;
			return (another.invocation == **invocationList.begin());
		} //==
		bool operator !=(const delegate<RET(PARAMS...)>& another) const { return !(*this == another); }

		multicast_delegate& operator +=(const multicast_delegate& another) {
			for (auto& item : another.invocationList) // clone, not copy; flattens hierarchy:
				this->invocationList.push_back(new typename delegate_base<RET(PARAMS...)>::InvocationElement(item->Object, item->Stub));
			return *this;
		} //operator +=

		template <typename LAMBDA> // template instantiation is not neededm, will be deduced/inferred:
		multicast_delegate& operator +=(const LAMBDA& lambda) {
			delegate<RET(PARAMS...)> d = delegate<RET(PARAMS...)>::template create<LAMBDA>(lambda);
			return *this += d;
		} //operator +=

		multicast_delegate& operator +=(const delegate<RET(PARAMS...)>& another) {
			if (another) return *this;
			this->invocationList.push_back(new typename delegate_base<RET(PARAMS...)>::InvocationElement(another.invocation.Object, another.invocation.Stub));
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

		void operator()(PARAMS... arg, delegate<void(size_t, RET*)> handler) const {
			operator() < decltype(handler) > (arg..., handler);
		} //operator()
		void operator()(PARAMS... arg, std::function<void(size_t, RET*)> handler) const {
			operator() < decltype(handler) > (arg..., handler);
		} //operator()

	private:

		std::list<typename delegate_base<RET(PARAMS...)>::InvocationElement*> invocationList;

	}; //class multicast_delegate
}