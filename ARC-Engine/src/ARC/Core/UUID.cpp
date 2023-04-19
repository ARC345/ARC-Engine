#include "arc_pch.h"

#include "UUID.h"

namespace ARC {
	static std::random_device sRandomDevice;
	static std::mt19937_64 sEngine(sRandomDevice());
	static std::uniform_int_distribution<TUUID> sUniformDistribution; // @TODO move to SRandom

	TUUID SUUID::Generate()
	{
		return sUniformDistribution(sEngine);
	}
}