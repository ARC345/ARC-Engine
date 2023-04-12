#pragma once

namespace ARC {
	struct SRandom
	{
	public:
		using randclass = std::mt19937;

		static void Init()
		{
			s_RandomEngine32.seed(std::random_device()());
		}
		template<typename T>
		static void Init(const T* _Seed)
		{
			if constexpr (_Seed == nullptr)
				Init();
			else
				s_RandomEngine32.seed(*_Seed);
		}

		template<typename T>
		static T Get() {
			return (T)s_Distribution(s_RandomEngine32);
		}
		template<typename T>
		static T Get(T _Min, T _Max) {
			return _Min + ((T)s_Distribution(s_RandomEngine32) / ((T)randclass::max() / (_Max - _Min)));
		}

		static inline float Float() { return SRandom::Get<float>(); }
		static inline float Float(float _Min, float _Max) { return SRandom::Get(_Min, _Max); }

		static inline int Int() { return SRandom::Get<int>(); }
		static inline int Int(int _Min, int _Max) { return SRandom::Get(_Min, _Max); }

	private:

		static inline std::mt19937 s_RandomEngine32;
		static inline std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};
}