#pragma once

#include "ARC.h"
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace ARC { class CParticleModifier2D; }
namespace ARC { class CParticle2D; }

namespace ARC {

	// @todo
	enum EVariationApplicationType : uint8_t {
		VariationPlus,
		VariationMinus,
		VariationHalfPlusHalfMinus,
		VariationPlusMinus
	};
	enum class EVariationApplicationTime : uint8_t {
		OnEmit,
		OnUpdate
	};

	class CParticle2D {
	public:
		CParticle2D() : bIsActive(1u) {}
		FVec3 Location = {0.f, 0.f, 0.f};
		float Rotation = 0.f;
		FVec2 Scale = {1.f, 1.f};
		FColor4 Color = FColor4::White();
		TRef<CTexture2D> Texture = nullptr;
		FVec3 Velocity = {0.f, 0.f, 0.f};
		float Life = 1.f;
		uint8_t bIsActive : 1;
		uint8_t bInitial : 1;		
	};
	class CParticleSystem2D
	{
	public:
		CParticleSystem2D(uint32_t _PoolSize = 1000);
		~CParticleSystem2D();
		void Emit();

		void OnUpdate(float _DeltaTime);
		void OnRender();

		void AddParticleModifier(EVariationApplicationTime VAP, CParticleModifier2D* Variation) { m_Modifiers[VAP].push_back(Variation); };

		CParticle2D Defaults;
	private:
		std::unordered_map<EVariationApplicationTime, std::vector<CParticleModifier2D*>> m_Modifiers;
		std::vector<CParticle2D> m_ParticlePool;
		uint32_t m_PoolIndex;
	};
}