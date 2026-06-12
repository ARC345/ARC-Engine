#include "arc_pch.h"

#include "UUID.h"

namespace arc {
	static std::random_device sRandomDevice;
	static std::mt19937_64 sEngine(sRandomDevice());
	static std::uniform_int_distribution<uuid> sUniformDistribution; // @TODO move to random

	uuid uuid::generate()
	{
		return sUniformDistribution(sEngine);
	}
}