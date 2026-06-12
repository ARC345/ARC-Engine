#pragma once
#include "../Helpers/Random.h"
namespace arc { class particle_2d; }

namespace arc {
	class particle_modifier_2d {
	public:
		virtual void ApplyModifier(particle_2d& particle) = 0;
		virtual void ApplyInitialModifier(particle_2d& particle) {
			ApplyModifier(particle);
		};
	};

	template<typename T>
	class particle_variation_modifier_2d : public particle_modifier_2d {
	public:
		particle_variation_modifier_2d(T particle_2d::* member_property, const T variation) : MemberProperty(member_property), Variation(variation) {};
		T particle_2d::* MemberProperty;
		T Variation;
		virtual inline void ApplyModifier(particle_2d& particle) override;
	};

	template<typename lerping_function=Linear, typename T=float>
	class particle_lifetime_modifier_2d : public particle_modifier_2d {
	public:
		particle_lifetime_modifier_2d(T particle_2d::* member_property, const T end) : MemberProperty(member_property), EndVal(end) {};
		T particle_2d::* MemberProperty;
		T InitialVal;
		T EndVal;
		float Scaling;
		virtual inline void ApplyModifier(particle_2d& particle) override;
		virtual inline void ApplyInitialModifier(particle_2d& particle) override;
	};

	template<typename T>
	void particle_variation_modifier_2d<T>::ApplyModifier(particle_2d& particle)
	{
		particle.*MemberProperty += (random::Float() - 0.5f) * Variation;
	}
	template<>
	void particle_variation_modifier_2d<float>::ApplyModifier(particle_2d& particle)
	{
		particle.*MemberProperty += (random::Float() - 0.5f) * Variation;
	}
	template<>
	void particle_variation_modifier_2d<vec2>::ApplyModifier(particle_2d& particle)
	{
		(particle.*MemberProperty).x += (random::Float() - 0.5f) * Variation.x;
		(particle.*MemberProperty).y += (random::Float() - 0.5f) * Variation.y;
	}
	template<>
	void particle_variation_modifier_2d<vec3>::ApplyModifier(particle_2d& particle)
	{
		(particle.*MemberProperty).x += (random::Float() - 0.5f) * Variation.x;
		(particle.*MemberProperty).y += (random::Float() - 0.5f) * Variation.y;
		(particle.*MemberProperty).z += (random::Float() - 0.5f) * Variation.z;
	}
	template<>
	void particle_variation_modifier_2d<vec4>::ApplyModifier(particle_2d& particle)
	{
		(particle.*MemberProperty).x += (random::Float() - 0.5f) * Variation.x;
		(particle.*MemberProperty).y += (random::Float() - 0.5f) * Variation.y;
		(particle.*MemberProperty).z += (random::Float() - 0.5f) * Variation.z;
		(particle.*MemberProperty).w += (random::Float() - 0.5f) * Variation.w;
	}

	template<>
	void particle_variation_modifier_2d<color4>::ApplyModifier(particle_2d& particle)
	{
		(particle.*MemberProperty).x += (random::Float() - 0.5f) * Variation.x;
		(particle.*MemberProperty).y += (random::Float() - 0.5f) * Variation.y;
		(particle.*MemberProperty).z += (random::Float() - 0.5f) * Variation.z;
		(particle.*MemberProperty).w += (random::Float() - 0.5f) * Variation.w;
	}

	template<typename lerping_function, typename T>
	void particle_lifetime_modifier_2d<lerping_function, T>::ApplyModifier(particle_2d& particle)
	{
		(particle.*MemberProperty) = math::InterpF<lerping_function, T>(InitialVal, EndVal, particle.Life/Scaling);
	}
	template<typename lerping_function, typename T>
	void particle_lifetime_modifier_2d<lerping_function, T>::ApplyInitialModifier(particle_2d& particle)
	{
		InitialVal = (particle.*MemberProperty);
		Scaling = particle.Life;
		ApplyModifier(particle);
	}
}