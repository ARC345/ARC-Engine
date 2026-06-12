#include "PCH/arc_pch.h"
#include "LifeSim2D.h"
#include "ARC/Scene/Scene.h"
#include "imgui/imgui.h"
#include "ARC/Scene/Entity.h"
#include "ARC/Scene/BasicComponents.h"
#include "ARC/Scene/SceneRegistry.h"
#include "ARC/Helpers/Random.h"
#include "ARC/Helpers/Math.h"
#include "ARC/Renderer/Renderer2D.h"
#include "imgui/imgui_internal.h"

namespace arc {
	// 2d only
	static bool bPause = false;

	static vec3 FindClosestPointOnLine(vec3 pLB, vec3 pLE, vec3 pP)
	{
		float LineLength = math::Dist(pLB, pLE);
		vec3 Vector = pP - pLB;
		vec3 LineDirection = (pLE - pLB) / LineLength;

		vec3 tmp(Vector * LineDirection);
		float Distance = tmp.x + tmp.y;

		if (Distance <= 0.f) return pLB;
		if (Distance >= LineLength) return pLE;
		return pLB + LineDirection * Distance;
	}

	static bool is_colliding(vec3 p1, vec3 p2, float pR1, float pR2) { return math::DistSqr(p1, p2) < math::Sqr(pR1+pR2); }

// basic simple light based organism simulator

	void energy_component::draw_properties_ui(entity & pEntity)
	{
		ImGui::DragFloat("Energy", &Energy);
	}


	void energy_component::serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Energy" << YAML::Value << Energy;
	}

	void energy_component::deserialize(YAML::Node& pData)
	{
		Energy = pData["Energy"].as<float>();
	}

	void organism_component::draw_properties_ui(entity& pEntity)
	{
		if (ImGui::CollapsingHeader("Data")) {
			ImGui::DragFloat("Health", &Data.Health, 0.01 * Data.MaxHealth, 0, Data.MaxHealth);
			ImGui::DragFloat("MaxHealth", &Data.MaxHealth);
			ImGui::DragFloat("Speed", &Data.Speed);

			ImGui::DragFloat("PhotoSynthRate", &Data.PhotoSynthRate);
			ImGui::DragFloat("HealRate", &Data.HealRate);
		}
		if (ImGui::CollapsingHeader("MutationData")) {
			ImGui::DragFloat("MutationMaxHealth", &MutationData.MaxHealth);
			ImGui::DragFloat("MutationSpeed", &MutationData.Speed);

			ImGui::DragFloat("MutationPhotoSynthRate", &MutationData.PhotoSynthRate);
			ImGui::DragFloat("MutationHealRate", &MutationData.HealRate);
		}
	}

	void organism_component::serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Health" << YAML::Value << Data.Health;
		pOut << YAML::Key << "Speed" << YAML::Value << Data.Speed;
	}

	void organism_component::deserialize(YAML::Node& pData)
	{
		Data.Health = pData["Health"].as<float>();
		Data.Speed = pData["Speed"].as<float>();
	}

	struct SResourceManager
	{
		static float Consume(float pAmt, float& pResource, float pReserve=0.f) {
			auto rval = math::Min(pResource-pReserve, pAmt);
			pResource -= rval;
			return rval;
		}
		/* returns delta resource(To)
		*  pConversionFactor == resource(To) per resource(From)
		*/
		static float Convert(float& pResourceFrom, float& pResourceTo, float pAmt, float pConversionFactor=1.f, float pResourceToCap=std::numeric_limits<float>::infinity()) {

			auto deltaTo = math::Min(
				math::Min(pResourceFrom, pAmt) * pConversionFactor,
				pResourceToCap - pResourceTo
			);
			pResourceTo += deltaTo;
			pResourceFrom -= deltaTo/pConversionFactor;
			return deltaTo;
		}


	};

	using SRM = SResourceManager;

	// @TODO add basic color schemeing and name bars for organisms

	entity life_sim_2d::MakeOrganism(const string& pName)
	{
		static auto CircleTex = texture_2d::create("assets/textures/circle-64.png");

		entity oE = active_scene_->create_entity(pName == "##Default" ? "Organism#" + std::to_string(e_index_++) : pName);
		auto& trans = oE.add_component<transform_2d_component>();
		oE.add_component<sprite_renderer_component>().Texture=CircleTex;
		auto& organism = oE.add_component<organism_component>();
		auto& energy = oE.add_component<energy_component>().Energy = organism.Data.InitialEnergy;
		auto& velocity = oE.add_component<velocity_component>().Velocity = vec2::ZeroVector();
		return oE;
	}

	float life_sim_2d::HealthToSize(float pHealth)
	{
		return math::Max(pHealth*pHealth/8000, min_size_);
	}

	float life_sim_2d::LightToRadiation(float pLight)
	{
		return pLight*0.68;
	}

	life_sim_2d::world_geo_data& life_sim_2d::get_geo_data_from_location(vec3 pLoc)
	{
		if (pLoc.x > world_max_bounds_.x || pLoc.x < -world_max_bounds_.x || pLoc.y > world_max_bounds_.y || pLoc.y < -world_max_bounds_.y) return empty_space_;

		float nearestX = float(std::floor((world_max_bounds_.x + pLoc.x) * grid_density_));
		float nearestY = float(std::floor((world_max_bounds_.y - pLoc.y) * grid_density_));
		
		return world_geo_data_[int(nearestX + 2 * world_max_bounds_.x * grid_density_ * nearestY)];;
	}

	const life_sim_2d::world_geo_data& life_sim_2d::get_geo_data_from_location(vec3 pLoc) const
	{
		if (pLoc.x > world_max_bounds_.x || pLoc.x < -world_max_bounds_.x || pLoc.y > world_max_bounds_.y || pLoc.y < -world_max_bounds_.y) return empty_space_;

		float nearestX = float(std::floor((world_max_bounds_.x + pLoc.x) * grid_density_));
		float nearestY = float(std::floor((world_max_bounds_.y - pLoc.y) * grid_density_));

		auto _ = int(nearestX + 2 * world_max_bounds_.x * grid_density_ * nearestY);

		return world_geo_data_[_];
	}

	void life_sim_2d::on_attach(ref<scene> pActiveScene)
	{
		set_scene(pActiveScene);
		
		scene_registry::setup_component<energy_component>();
		scene_registry::setup_component<organism_component>();
		scene_registry::setup_component<velocity_component>();

		{
		auto camE = active_scene_->create_entity("Camera");
		auto& trans = camE.add_component<transform_2d_component>();
		auto& cam = camE.add_component<camera_component>();
		cam.Primary = true;
		cam.Camera.set_orthographic_size((world_max_bounds_.MaxComponent()*2.f));
		}

		for (int i = 0; i<1000; i++)
			MakeOrganism();

	}

	void life_sim_2d::on_detach()
	{
	}

	void life_sim_2d::on_update_editor(float pDeltaTime)
	{
		if (bPause) return;
		static const vec3 Mask = vec3(1, 1, 0);

	}

	void life_sim_2d::on_update_runtime(float pDeltaTime)
	{
		if (bPause) return;
		static const vec3 Mask = vec3(1, 1, 0);

		pDeltaTime*= time_multiplier_;
		time_ += pDeltaTime;

		std::set<entity> DeathRow;

		if (auto E = active_scene_->get_primary_camera_entity())
		{
			static bool itrlDO2 = true;
		
			renderer_2d::begin_scene(E.get_component<camera_component>().Camera, E.get_component<transform_2d_component>().Transform);
			renderer_2d::DrawRect(vec3::ZeroVector(), 0.f, world_max_bounds_ * 2.f, color4::Yellow()*0.7f);
			renderer_2d::DrawRect(vec3::ZeroVector(), 0.f, (world_max_bounds_ * 2.f) + 1, color4::Blue() * 0.9f);

			{
				auto view = active_scene_->FilterByComponents<transform_2d_component, velocity_component>();
				std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&](auto e) {
					auto& transformComponent = view.get<transform_2d_component>(e);
					auto& velocityComponent = view.get<velocity_component>(e);

					(vec2&)transformComponent.Transform.Location += velocityComponent.Velocity*pDeltaTime;
				});	
			}
			static float savedDT = 0;
			savedDT += pDeltaTime;
			{
				auto view= active_scene_->FilterByComponents<transform_2d_component, organism_component>();
				std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&](auto e) {
					auto& transformComponent = view.get<transform_2d_component>(e);
					auto& organism = view.get<organism_component>(e);
					if (transformComponent.Transform.Location.x > world_max_bounds_.x) transformComponent.Transform.Location.x = -world_max_bounds_.x;
					if (transformComponent.Transform.Location.x < -world_max_bounds_.x) transformComponent.Transform.Location.x = world_max_bounds_.x;
					if (transformComponent.Transform.Location.y > world_max_bounds_.y) transformComponent.Transform.Location.y = -world_max_bounds_.y;
					if (transformComponent.Transform.Location.y < -world_max_bounds_.y) transformComponent.Transform.Location.y = world_max_bounds_.y;

					if (savedDT > random::Float(1, 1.5)) {
						transformComponent.Transform.Scale = vec2(HealthToSize(organism.Data.Health));

						auto E = entity(e, active_scene_.get());
						auto& energy = E.get_component<energy_component>();
						float speed = 0.f;
						SRM::Convert(energy.Energy, speed, energy.Energy, speed_per_energy_, organism.Data.Speed);

						E.get_component<velocity_component>().Velocity = vec2(math::Sin(random::Float(-1, 1) * PIf), math::Cos(random::Float(-1, 1) * PIf)) * speed;
						if (0.01f > energy.Energy) DeathRow.emplace(E);
					}
				});
			}
			if (savedDT>=1.2f) savedDT = 0.f;

			if (fmodf(time_, 10.f) < 0.1f) {
				if (itrlDO2) {
					std::for_each(std::execution::par_unseq, world_geo_data_, world_geo_data_+grid_count_, [&](auto&& i) {
						auto lI = random::Float(min_light_, 5);
						i = life_sim_2d::world_geo_data(lI);
					});
					itrlDO2 = false;
				}
			}
			else
				itrlDO2 = true;
// 			for (int i = 0; i < grid_count_; i++) {
// 				auto xScale = world_max_bounds_.x / (world_max_bounds_.x + world_max_bounds_.y);
// 				renderer_2d::draw_quad(
// 					vec3(
// 						(i % int(world_max_bounds_.x * 2 * grid_density_) - world_max_bounds_.x),
// 						xScale*(i / int(world_max_bounds_.y * 2 * grid_density_)) - world_max_bounds_.y,
// 						-0.1),
// 
// 					0,
// 					vec2(1, 1-xScale) / grid_density_,
// 					transparency_type::Opaque,
// 					color4::Yellow() * world_geo_data_[i].LightIntensity / 5);
// 			}

			auto view = active_scene_->FilterByComponents<organism_component, energy_component>();
			std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&](auto e) {
				// Photosynthesis

				auto& organism = view.get<organism_component>(e);
				auto& energy = view.get<energy_component>(e);
				float photoAbsorb = get_light_intensity_at_location(E.get_component<transform_2d_component>().Transform.Location);
				energy.Energy = math::Clamp(energy.Energy + (photoAbsorb * organism.Data.PhotoSynthRate * pDeltaTime), 0.f, energy.MaxEnergy);
				// @TODO release appropriate energy amt
				//if (0.01f > energy.Energy) ARC_CORE_INFO("death {}", int(e)); DeathRow.emplace(E);
				
				//energy leak
				SRM::Consume(
					math::Interp<EaseOut<2>>(
						0.f,
						energy.MaxEnergy * energy_leak_multiplier_,
						organism.Data.Health / organism.Data.MaxHealth) * pDeltaTime,
					energy.Energy
				);
				//if (0.01f > energy.Energy) ARC_CORE_INFO("death {}", int(e)); DeathRow.emplace(E);
				
				// health regen
				SRM::Convert(
					energy.Energy,
					organism.Data.Health,
					energy.Energy * organism.Data.HealRate * pDeltaTime,
					health_per_energy_,
					organism.Data.MaxHealth
				);

				//if (0.01f > energy.Energy) ARC_CORE_INFO("death {}", int(e)); DeathRow.emplace(E);
			});
			auto iter = active_scene_->FilterByComponents<organism_component>().each();

			for (auto i=iter.begin(); i!=iter.end(); i++)
			{
				auto icpy = i;
				for (auto ii = ++icpy; ii != iter.end(); ii++)
				{
					if (i==ii)
					{
						ARC_CORE_ASSERT("");
					}
				}
			}

			// reproduction
			{
				auto view = active_scene_->FilterByComponents<organism_component, energy_component>();
				std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&](auto e) {
					// Photosynthesis

					auto& organism = view.get<organism_component>(e);
					auto& energy = view.get<energy_component>(e);	auto E = entity(e, active_scene_.get());
					if (energy.Energy == energy.MaxEnergy && energy.Energy >= min_reproduction_energy_cost_) // does not reproduce until max energy
					{
						// reproduce
						auto EChild = MakeOrganism(E.get_name()+"_child" + std::to_string(organism.NextChildsIndex++));
						auto& ec_organismComp = EChild.get_component<organism_component>();
						auto& ec_energyComp = EChild.get_component<energy_component>();
					
						EChild.get_component<transform_2d_component>().Transform.Location = E.get_component<transform_2d_component>().Transform.Location;
						EChild.get_component<transform_2d_component>().Transform.Location.y += E.get_component<transform_2d_component>().Transform.Scale.MaxComponent() + EChild.get_component<transform_2d_component>().Transform.Scale.MaxComponent() + 0.1f;

						float RadiationFactor = LightToRadiation(get_light_intensity_at_location(E.get_component<transform_2d_component>().Transform.Location));

						ec_organismComp.Data.MaxHealth += random::Float(-ec_organismComp.MutationData.MaxHealth, ec_organismComp.MutationData.MaxHealth)*RadiationFactor;
						ec_organismComp.Data.Speed += random::Float(-ec_organismComp.MutationData.Speed, ec_organismComp.MutationData.Speed) * RadiationFactor;
						ec_organismComp.Data.PhotoSynthRate += random::Float(-ec_organismComp.MutationData.PhotoSynthRate, ec_organismComp.MutationData.PhotoSynthRate) * RadiationFactor;
						ec_organismComp.Data.HealRate += random::Float(-ec_organismComp.MutationData.HealRate, ec_organismComp.MutationData.HealRate) * RadiationFactor;
						ec_organismComp.Data.InitialEnergy += random::Float(-ec_organismComp.MutationData.InitialEnergy, ec_organismComp.MutationData.InitialEnergy) * RadiationFactor;
						ec_organismComp.Data.PostReproductionEnergyReserve += random::Float(-ec_organismComp.MutationData.PostReproductionEnergyReserve, ec_organismComp.MutationData.PostReproductionEnergyReserve) * RadiationFactor;
						ec_energyComp.MaxEnergy += random::Float(-ec_organismComp.MutationData.MaxEnergy, ec_organismComp.MutationData.MaxEnergy) * RadiationFactor;

						ec_organismComp.Data.MaxHealth = math::Max(0.f, ec_organismComp.Data.MaxHealth);
						ec_organismComp.Data.Speed = math::Max(0.f, ec_organismComp.Data.Speed);
						ec_organismComp.Data.PhotoSynthRate = math::Max(0.f, ec_organismComp.Data.PhotoSynthRate);
						ec_organismComp.Data.HealRate = math::Max(0.f, ec_organismComp.Data.HealRate);
						ec_organismComp.Data.InitialEnergy = math::Max(0.f, ec_organismComp.Data.InitialEnergy);
						ec_organismComp.Data.PostReproductionEnergyReserve = math::Max(0.f, ec_organismComp.Data.PostReproductionEnergyReserve);
						ec_energyComp.MaxEnergy = math::Max(0.f, ec_energyComp.MaxEnergy);

						ec_organismComp.Data.Health = 0.f;

						SRM::Consume(min_reproduction_energy_cost_, energy.Energy, organism.Data.PostReproductionEnergyReserve);
						SRM::Consume(ec_organismComp.Data.InitialEnergy, energy.Energy, organism.Data.PostReproductionEnergyReserve);
						SRM::Convert(energy.Energy, ec_organismComp.Data.Health, energy.Energy - organism.Data.PostReproductionEnergyReserve, health_per_energy_, ec_organismComp.Data.MaxHealth);
					}
					if (0.01f > energy.Energy) { 
						ARC_CORE_INFO("reproduction death{}", int(e)); DeathRow.emplace(E);
					}
				});
			}

			for (auto i : DeathRow) {
				i.Kill();
			}


			renderer_2d::end_scene();
		}
	}

	void life_sim_2d::on_gui_render()
	{
		ImGui::begin("LS2D");

		static vec3 tmp = vec3::ZeroVector();
		float tmp2 = LightToRadiation(get_light_intensity_at_location(tmp)) / 5;
		float tmp3 = get_light_intensity_at_location(tmp);
		ImGui::DragFloat3("Rad@Loc", tmp.Data());
		ImGui::DragFloat("Light@Loc", &tmp3, ImGuiSliderFlags_ReadOnly);
		ImGui::DragFloat("LightTmp", &tmp2, ImGuiSliderFlags_ReadOnly);
		
		ImGui::DragFloat("Time Multiplier", &time_multiplier_);
		ImGui::DragFloat("World Time", &time_);
		float abcd = TimeToTimeOfDay();
		ImGui::DragFloat("Time of day", &abcd);
		abcd = get_light_intensity();
		ImGui::DragFloat("LightIntensity", &abcd);
		abcd = LightToRadiation(get_light_intensity());
		ImGui::DragFloat("Radiation", &abcd);

		string timeofday = abcd > 0.2f ? "Morning" : "Night";

		ImGui::Text(timeofday.c_str());

		ImGui::end();
	}

	void life_sim_2d::on_event(event& pEvent)
	{
	}

	void life_sim_2d::set_scene(ref<scene> pScene)
	{
		active_scene_ = pScene;
		time_ = 0.f;
		delete[] world_geo_data_;
		grid_count_ = int(4.f * world_max_bounds_.x* (world_max_bounds_.y+0.5) * grid_density_)+1;
		world_geo_data_ = new world_geo_data[grid_count_];

		for (int i = 0; i < grid_count_; i++)
			world_geo_data_[i] = life_sim_2d::world_geo_data(random::Float(min_light_, 5));

	}

	float life_sim_2d::TimeToTimeOfDay() const
	{
		return fmodf(time_, day_length_);
	}

	float life_sim_2d::get_light_intensity() const
	{
		auto _ = math::Clamp(
			(math::Sin(math::Interp<Linear>(0.f, 1.2f * PIf, TimeToTimeOfDay() / day_length_))) * light_multiplier_, 
			min_light_,
			light_multiplier_
		);
		return _;
	}

	float life_sim_2d::get_light_intensity_at_location(vec3 pLoc) const
	{
		return get_light_intensity()* get_geo_data_from_location(pLoc).LightIntensity;
	}
}