#pragma once
#include <string>

namespace ARC {
	template <typename T>
	struct TStringBase
	{
		using string_type = std::string;

		template <typename... Args, typename = decltype(string_type(std::declval<Args>()...))>
		TStringBase(Args &&... _A) : mStr(std::forward<Args>(_A)...)
		{
		}
		TStringBase(const TStringBase& str) : mStr(str.mStr, 0, mStr.npos) {}
		TStringBase(const TStringBase& str, size_t pos, size_t n) : mStr(str.mStr, pos, n) {}

		TStringBase& operator= (const TStringBase& str) { mStr = str.mStr; return *this; }
		TStringBase& operator= (const T* s) { mStr = s; return *this; }
		TStringBase& operator= (T c) { mStr = c; return *this; }

		// iterators:
		inline auto begin() { return mStr.begin(); }
		inline auto begin() const { return mStr.begin(); }

		inline auto end() { return mStr.end(); }
		inline auto end() const { return mStr.end(); }

		inline auto rbegin() { return mStr.rbegin(); }
		inline auto rbegin() const { return mStr.rbegin(); }

		inline auto rend() { return mStr.rend(); }
		inline auto rend() const { return mStr.rend(); }

		// capacity:
		inline size_t Len() const { return mStr.length(); }
		inline size_t MaxSize() const { return mStr.max_size(); }
		inline size_t Capacity() const { return mStr.capacity(); }

		inline void Clear() { mStr.erase(base_iter(begin()), base_iter(end())); } // no clear() in MSVC++ 6.0
		inline bool Empty() const { return mStr.empty(); }

		template <typename... Args>
		void Resize(Args &&... args) { mStr.resize(std::forward<Args>(args)...); }

		template <typename... Args>
		void Reserve(Args &&... args) { mStr.reserve(std::forward<Args>(args)...); }

		// element access:
		template <typename... Args>
		auto operator[] (size_t pos) { return mStr[pos]; }
		auto operator[] (size_t pos) const { return mStr[pos]; }
		string_type::reference       At(size_t n) { return mStr.at(n); }
		string_type::const_reference At(size_t n) const { return mStr.at(n); }

		// modifiers:
		TStringBase& operator+= (const TStringBase& str) { mStr += (str.mStr); return *this; }
		TStringBase& operator+= (const T* s) { mStr += (s); return *this; }
		TStringBase& operator+= (T c) { mStr += (c); return *this; }

		template <typename... Args>
		TStringBase& Append(Args &&... args) { mStr.append(std::forward<Args>(args)...); return *this; }
		template <typename... Args>
		TStringBase& Assign(Args &&... args) { mStr.assign(std::forward<Args>(args)...); return *this; }
		template <typename... Args>
		TStringBase& Insert(Args &&... args) { mStr.insert(std::forward<Args>(args)...); return *this; }
		template <typename... Args>
		TStringBase& Erase(Args &&... args) { mStr.erase(std::forward<Args>(args)...); return *this; }
		template <typename... Args>
		TStringBase& Replace(Args &&... args) { mStr.replace(std::forward<Args>(args)...); return *this; }

		void PushBack(const T c) { mStr += (c); } // // no push_back() in MSVC++ 6.0

		template <typename... Args>
		size_t Copy(Args &&... args) const { return mStr.copy(std::forward<Args>(args)...); }

		void Swap(TStringBase& str) { mStr.swap(str.CStr()); }
		friend inline void Swap(TStringBase& lhs, TStringBase& rhs) { lhs.Swap(rhs); }

		// string operations:
		const T* CStr() const { return mStr.c_str(); }
		const T* Data() const { return mStr.data(); }

		string_type::allocator_type GetAllocator() const { return mStr.get_allocator(); }

		template <typename... Args>
		size_t Find(Args &&... args) const
		{
			return mStr.find(std::forward<Args>(args));
		}

		template <typename... Args>
		size_t ReverseFind(Args &&... args) const
		{
			return mStr.rfind(std::forward<Args>(args)...);
		}

		template <typename... Args>
		size_t FindFirstOf(Args &&... args) const
		{
			return mStr.find_first_of(std::forward<Args>(args)...);
		}
		template <typename... Args>
		size_t FindLastOf(Args &&... args) const
		{
			return mStr.find_last_of(std::forward<Args>(args)...);
		}
		template <typename... Args>
		size_t FindFirstNotOf(Args &&... args) const
		{
			return mStr.find_first_not_of(std::forward<Args>(args)...);
		}
		template <typename... Args>
		size_t FindLastNotOf(Args &&... args) const
		{
			return mStr.find_last_not_of(std::forward<Args>(args)...);
		}

		TStringBase SubStr(size_t pos = 0) const { return mStr.substr(pos); }
		TStringBase SubStr(size_t pos, size_t n) const { return mStr.substr(pos, n); }

		static constexpr inline size_t& NPos() { return mStr.npos; }

		template <typename... Args>
		int Compare(Args &&... args) const
		{
			return mStr.compare(std::forward<Args>(args));
		}

		friend inline TStringBase operator+ (const TStringBase& lhs, const TStringBase& rhs) {
			return TStringBase(lhs, rhs, false);
		}
		friend inline TStringBase operator+ (const T* lhs, const TStringBase& rhs) {
			return TStringBase(lhs, rhs);
		}
		friend inline TStringBase operator+ (T lhs, const TStringBase& rhs) {
			return TStringBase(lhs, rhs);
		}
		friend inline TStringBase operator+ (const TStringBase& lhs, const T* rhs) {
			return TStringBase(lhs, rhs);
		}
		friend inline TStringBase operator+ (const TStringBase& lhs, T rhs) {
			return TStringBase(lhs, rhs);
		}

		friend inline bool operator== (const TStringBase& lhs, const TStringBase& rhs) { return lhs.mStr == rhs.mStr; }
		friend inline bool operator== (const T* lhs, const TStringBase& rhs) { return lhs == rhs.mStr; }
		friend inline bool operator== (const TStringBase& lhs, const T* rhs) { return lhs.mStr == rhs; }
	
		friend inline bool operator<  (const TStringBase& lhs, const TStringBase& rhs) { return lhs.mStr < rhs.mStr; }
		friend inline bool operator<  (const TStringBase& lhs, const T* rhs) { return lhs.mStr < rhs; }
		friend inline bool operator<  (const T* lhs, const TStringBase& rhs) { return lhs < rhs.mStr; }

		friend inline bool operator!= (const TStringBase& lhs, const TStringBase& rhs) { return !(lhs == rhs); }
		friend inline bool operator!= (const T* lhs, const TStringBase& rhs) { return !(lhs == rhs); }
		friend inline bool operator!= (const TStringBase& lhs, const T* rhs) { return !(lhs == rhs); }
	
		friend inline bool operator>  (const TStringBase& lhs, const TStringBase& rhs) { return rhs < lhs; }
		friend inline bool operator>  (const TStringBase& lhs, const T* rhs) { return rhs < lhs; }
		friend inline bool operator>  (const T* lhs, const TStringBase& rhs) { return rhs < lhs; }
	
		friend inline bool operator<= (const TStringBase& lhs, const TStringBase& rhs) { return !(rhs < lhs); }
		friend inline bool operator<= (const TStringBase& lhs, const T* rhs) { return !(rhs < lhs); }
		friend inline bool operator<= (const T* lhs, const TStringBase& rhs) { return !(rhs < lhs); }
	
		friend inline bool operator>= (const TStringBase& lhs, const TStringBase& rhs) { return !(lhs < rhs); }
		friend inline bool operator>= (const TStringBase& lhs, const T* rhs) { return !(lhs < rhs); }
		friend inline bool operator>= (const T* lhs, const TStringBase& rhs) { return !(lhs < rhs); }

		friend inline std::basic_ostream<T, string_type::traits_type>&
			operator<< (std::basic_ostream<T, string_type::traits_type>& os, const TStringBase& str) {
			return os << str.mStr;
		}

		friend inline std::basic_istream <T, string_type::traits_type>&
			operator>> (std::basic_istream<T, string_type::traits_type>& is, TStringBase& str) {
			is >> str.mStr;
			return is;
		}

		friend inline std::basic_istream<T, string_type::traits_type>&
			getline(std::basic_istream<T, string_type::traits_type>& is, TStringBase& str, T delim) {
			std::getline(is, str.mStr, delim);
			return is;
		}

		friend inline std::basic_istream<T, string_type::traits_type>&
			getline(std::basic_istream<T, string_type::traits_type>& is, TStringBase& str) {
			return getline(is, str, is.widen('\n'));
		}

	private:
		// private constructors, used only to facilitate RVO for operator+() 

		// the third argument is not used, it disambiguates parsing for older compilers
		explicit TStringBase(const TStringBase& lhs, const TStringBase& rhs, bool/*not used*/) {
			mStr.reserve(lhs.Len() + rhs.Len());
			mStr.append(lhs.mStr).append(rhs.mStr);
		}
		explicit TStringBase(const T* lhs, const TStringBase& rhs) {
			assert(lhs);
			size_t lhsLen = std::char_traits<T>::length(lhs);
			mStr.reserve(lhsLen + rhs.Len());
			mStr.append(lhs, lhsLen).append(rhs.mStr);
		}
		explicit TStringBase(T lhs, const TStringBase& rhs) {
			mStr.reserve(1 + rhs.Len());
			mStr.append(1, lhs).append(rhs.mStr);
		}
		explicit TStringBase(const TStringBase& lhs, const T* rhs) {
			assert(rhs);
			size_t rhsLen = std::char_traits<T>::length(rhs);
			mStr.reserve(lhs.Len() + rhsLen);
			mStr.append(lhs.mStr).append(rhs, rhsLen);
		}
		explicit TStringBase(const TStringBase& lhs, T rhs) {
			mStr.reserve(lhs.Len() + 1);
			mStr.append(lhs.mStr).append(1, rhs);
		}

		string_type::iterator base_iter(string_type::iterator iter) { return iter; }

	private:
		string_type mStr;
	};

	using TString = TStringBase<char>;
	}