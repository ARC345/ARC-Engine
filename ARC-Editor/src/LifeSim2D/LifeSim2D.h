#pragma once
#include "ARC/Renderer/Layer.h"
#include "ARC/Scene/Component.h"

namespace arc { class scene; }
namespace arc { class event; }

namespace arc
{
	struct energy_component : public component_base
	{
		float Energy = 10.f;
		float MaxEnergy = 100.f;

		virtual void draw_properties_ui(entity& pEntity) override;
		virtual void serialize(YAML::Emitter& pOut) override;
		virtual void deserialize(YAML::Node& pData) override;
		virtual u32 get_flags() override { return Flags; };

		static inline u32 Flags = ecf::DefaultComponentFlags | ecf::Serializable;
	};

	struct organism_component : public component_base
	{
		struct organism_data {
			float MaxHealth = 100;
			float Health = MaxHealth; // energy leak multiplier = 1 - health
			float Speed = 1.f; // defines how fast can organism move// acceleration consumes energy// unused
			float PhotoSynthRate = 0.69;
			float HealRate = 1;

			// @TODO gui
			float PostReproductionEnergyReserve = 20.f;
			float InitialEnergy = 10;
		};

		struct organism_mutation_data {
			float MaxHealth = 1;
			float Speed = 0.1;
			float PhotoSynthRate = 0.1;
			float HealRate = 0.1;
			float PostReproductionEnergyReserve = 1;
			float InitialEnergy = 1;
			float MaxEnergy = 1;

			// @TODO
			//organism_data InitialDataMultiplier= organism_data(0.2f, 0.2f, 1.2f, 1.5f, 0.2f, 1.f); 
			//float TimeTillFullGrowth = 2.f; // in days
		};

		organism_data Data;
		organism_mutation_data MutationData;

		u32 NextChildsIndex = 0; // next name of child && reward

		virtual void draw_properties_ui(entity& pEntity) override;
		virtual void serialize(YAML::Emitter& pOut) override;
		virtual void deserialize(YAML::Node& pData) override;
		virtual u32 get_flags() override { return Flags; };

		static inline u32 Flags = ecf::DefaultComponentFlags | ecf::Serializable;
	};

	struct velocity_component : public component_base { 
		vec2 Velocity;
		virtual u32 get_flags() override { return Flags; };
		static inline u32 Flags = 0;
	};

	class life_sim_2d
	{

	public:
		void on_attach(ref<scene> pActiveScene);
		void on_detach();
		void on_update_editor(float pDeltaTime);
		void on_update_runtime(float pDeltaTime);
		void on_gui_render();
		void on_event(event& pEvent);

		void set_scene(ref<scene> pScene);

		float TimeToTimeOfDay() const;
		float get_light_intensity() const;
		float get_light_intensity_at_location(vec3 pLoc) const;

		entity MakeOrganism(const string& pName = "##Default");

		float HealthToSize(float pHealth);
		float LightToRadiation(float pLight);
		struct world_geo_data { float LightIntensity = 1.f; };

		world_geo_data& get_geo_data_from_location(vec3 pLoc);
		const world_geo_data& get_geo_data_from_location(vec3 pLoc) const;

	private:
		static inline world_geo_data empty_space_ = world_geo_data(0.f);
		world_geo_data* world_geo_data_;
		int grid_density_ = 1; // grid per unit area
		int grid_count_ = 0; // grid per unit area
		vec2 world_max_bounds_= vec2(60.f, 40.f);

		float time_ = 0.f;
		float time_multiplier_ = 1.f;
		float day_length_ = 100.f; // length of 1 day
		float light_multiplier_ = 1.f;
		float min_light_ = 0.01;
		float energy_leak_multiplier_ = 0.01f;
		float health_per_energy_ = 2.f;
		float min_reproduction_energy_cost_ = 15.f;
		float speed_per_energy_ = 10.f;
		float min_size_ = 0.35f;

		u32 e_index_ = 0;

		ref<scene> active_scene_;
	};
}
