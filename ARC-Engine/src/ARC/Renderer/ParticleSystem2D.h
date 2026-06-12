#pragma once

// @TODO Rework

#include "ARC.h"
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace arc { class particle_modifier_2d; }
namespace arc { class particle_2d; }

namespace arc {

	// @todo
	enum variation_application_type : uint8_t {
		VariationPlus,
		VariationMinus,
		VariationHalfPlusHalfMinus,
		VariationPlusMinus
	};
	enum class variation_application_time : uint8_t {
		OnEmit,
		on_update
	};

	class particle_2d {
	public:
		particle_2d() : bIsActive(1u) {}
		vec3 Location = {0.f, 0.f, 0.f};
		float Rotation = 0.f;
		vec2 Scale = {1.f, 1.f};
		color4 Color = color4::White();
		ref<texture_2d> Texture = nullptr;
		vec3 Velocity = {0.f, 0.f, 0.f};
		float Life = 1.f;
		uint8_t bIsActive : 1;
		uint8_t bInitial : 1;		
	};
	class particle_system_2d
	{
	public:
		particle_system_2d(uint32_t pool_size = 1000);
		~particle_system_2d();
		void Emit();

		void on_update(float delta_time);
		void OnRender();

		void AddParticleModifier(variation_application_time VAP, particle_modifier_2d* Variation) { modifiers_[VAP].push_back(Variation); };

		particle_2d Defaults;
	private:
		std::unordered_map<variation_application_time, std::vector<particle_modifier_2d*>> modifiers_;
		std::vector<particle_2d> particle_pool_;
		uint32_t pool_index_;
	};
}