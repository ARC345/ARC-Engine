#pragma once

namespace arc {
	struct random
	{
	public:
		using randclass = std::mt19937;

		static void init()
		{
			random_engine32.seed(std::random_device()());
		}
		template<typename T>
		static void init(const T* seed)
		{
			if constexpr (seed == nullptr)
				init();
			else
				random_engine32.seed(*seed);
		}

		template<typename T>
		static T Get() {
			return (T)distribution(random_engine32);
		}
		template<typename T>
		static T Get(T min, T max) {
			return min + ((T)distribution(random_engine32) / ((T)randclass::max() / (max - min)));
		}

		static inline float Float() { return random::Get<float>(); }
		static inline float Float(float min, float max) { return random::Get(min, max); }

		static inline int Int() { return random::Get<int>(); }
		static inline int Int(int min, int max) { return random::Get(min, max); }

	private:

		static inline std::mt19937 random_engine32;
		static inline std::uniform_int_distribution<std::mt19937::result_type> distribution;
	};
}