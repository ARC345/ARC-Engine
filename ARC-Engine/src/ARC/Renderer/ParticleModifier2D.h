#pragma once
namespace ARC { class CParticle2D; }

namespace ARC {
	class CParticleModifier2D {
	public:
		virtual void ApplyModifier(CParticle2D& _Particle) = 0;
		virtual void ApplyInitialModifier(CParticle2D& _Particle) {
			ApplyModifier(_Particle);
		};
	};

	template<typename T>
	class CParticleVariationModifier2D : public CParticleModifier2D {
	public:
		CParticleVariationModifier2D(T CParticle2D::* _MemberProperty, const T _Variation) : MemberProperty(_MemberProperty), Variation(_Variation) {};
		T CParticle2D::* MemberProperty;
		T Variation;
		virtual inline void ApplyModifier(CParticle2D& _Particle) override;
	};

	template<typename LerpingFunction=Linear, typename T=float>
	class CParticleLifetimeModifier2D : public CParticleModifier2D {
	public:
		CParticleLifetimeModifier2D(T CParticle2D::* _MemberProperty, const T _End) : MemberProperty(_MemberProperty), EndVal(_End) {};
		T CParticle2D::* MemberProperty;
		T InitialVal;
		T EndVal;
		float Scaling;
		virtual inline void ApplyModifier(CParticle2D& _Particle) override;
		virtual inline void ApplyInitialModifier(CParticle2D& _Particle) override;
	};

	template<typename T>
	void CParticleVariationModifier2D<T>::ApplyModifier(CParticle2D& _Particle)
	{
		_Particle.*MemberProperty += (HPR::Random::Float() - 0.5f) * Variation;
	}
	template<>
	void CParticleVariationModifier2D<float>::ApplyModifier(CParticle2D& _Particle)
	{
		_Particle.*MemberProperty += (HPR::Random::Float() - 0.5f) * Variation;
	}
	template<>
	void CParticleVariationModifier2D<FVec2>::ApplyModifier(CParticle2D& _Particle)
	{
		(_Particle.*MemberProperty).x += (HPR::Random::Float() - 0.5f) * Variation.x;
		(_Particle.*MemberProperty).y += (HPR::Random::Float() - 0.5f) * Variation.y;
	}
	template<>
	void CParticleVariationModifier2D<FVec3>::ApplyModifier(CParticle2D& _Particle)
	{
		(_Particle.*MemberProperty).x += (HPR::Random::Float() - 0.5f) * Variation.x;
		(_Particle.*MemberProperty).y += (HPR::Random::Float() - 0.5f) * Variation.y;
		(_Particle.*MemberProperty).z += (HPR::Random::Float() - 0.5f) * Variation.z;
	}
	template<>
	void CParticleVariationModifier2D<FVec4>::ApplyModifier(CParticle2D& _Particle)
	{
		(_Particle.*MemberProperty).x += (HPR::Random::Float() - 0.5f) * Variation.x;
		(_Particle.*MemberProperty).y += (HPR::Random::Float() - 0.5f) * Variation.y;
		(_Particle.*MemberProperty).z += (HPR::Random::Float() - 0.5f) * Variation.z;
		(_Particle.*MemberProperty).w += (HPR::Random::Float() - 0.5f) * Variation.w;
	}

	template<>
	void CParticleVariationModifier2D<FColor>::ApplyModifier(CParticle2D& _Particle)
	{
		(_Particle.*MemberProperty).x += (HPR::Random::Float() - 0.5f) * Variation.x;
		(_Particle.*MemberProperty).y += (HPR::Random::Float() - 0.5f) * Variation.y;
		(_Particle.*MemberProperty).z += (HPR::Random::Float() - 0.5f) * Variation.z;
		(_Particle.*MemberProperty).w += (HPR::Random::Float() - 0.5f) * Variation.w;
	}

	template<typename LerpingFunction, typename T>
	void CParticleLifetimeModifier2D<LerpingFunction, T>::ApplyModifier(CParticle2D& _Particle)
	{
		(_Particle.*MemberProperty) = Math::InterpF<LerpingFunction, T>(InitialVal, EndVal, _Particle.Life/Scaling);
	}
	template<typename LerpingFunction, typename T>
	void CParticleLifetimeModifier2D<LerpingFunction, T>::ApplyInitialModifier(CParticle2D& _Particle)
	{
		InitialVal = (_Particle.*MemberProperty);
		Scaling = _Particle.Life;
		ApplyModifier(_Particle);
	}
}