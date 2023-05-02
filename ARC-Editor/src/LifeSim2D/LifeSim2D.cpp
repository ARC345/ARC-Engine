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

namespace ARC {
	// 2d only
	static bool bPause = false;

	static FVec3 FindClosestPointOnLine(FVec3 pLB, FVec3 pLE, FVec3 pP)
	{
		float LineLength = SMath::Dist(pLB, pLE);
		FVec3 Vector = pP - pLB;
		FVec3 LineDirection = (pLE - pLB) / LineLength;

		FVec3 tmp(Vector * LineDirection);
		float Distance = tmp.x + tmp.y;

		if (Distance <= 0.f) return pLB;
		if (Distance >= LineLength) return pLE;
		return pLB + LineDirection * Distance;
	}

	static bool IsColliding(FVec3 p1, FVec3 p2, float pR1, float pR2) { return SMath::DistSqr(p1, p2) < SMath::Sqr(pR1+pR2); }

// basic simple light based organism simulator

	void CEnergyComponent::DrawPropertiesUI(CEntity & pEntity)
	{
		ImGui::DragFloat("Energy", &Energy);
	}


	void CEnergyComponent::Serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Energy" << YAML::Value << Energy;
	}

	void CEnergyComponent::Deserialize(YAML::Node& pData)
	{
		Energy = pData["Energy"].as<float>();
	}

	void COrganismComponent::DrawPropertiesUI(CEntity& pEntity)
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

	void COrganismComponent::Serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Health" << YAML::Value << Data.Health;
		pOut << YAML::Key << "Speed" << YAML::Value << Data.Speed;
	}

	void COrganismComponent::Deserialize(YAML::Node& pData)
	{
		Data.Health = pData["Health"].as<float>();
		Data.Speed = pData["Speed"].as<float>();
	}

	struct SResourceManager
	{
		static float Consume(float pAmt, float& pResource, float pReserve=0.f) {
			auto rval = SMath::Min(pResource-pReserve, pAmt);
			pResource -= rval;
			return rval;
		}
		/* returns delta resource(To)
		*  pConversionFactor == resource(To) per resource(From)
		*/
		static float Convert(float& pResourceFrom, float& pResourceTo, float pAmt, float pConversionFactor=1.f, float pResourceToCap=std::numeric_limits<float>::infinity()) {

			auto deltaTo = SMath::Min(
				SMath::Min(pResourceFrom, pAmt) * pConversionFactor,
				pResourceToCap - pResourceTo
			);
			pResourceTo += deltaTo;
			pResourceFrom -= deltaTo/pConversionFactor;
			return deltaTo;
		}


	};

	using SRM = SResourceManager;

	// @TODO add basic color schemeing and name bars for organisms

	CEntity CLifeSim2D::MakeOrganism(const TString& pName)
	{
		static auto CircleTex = CTexture2D::Create("assets/textures/circle-64.png");

		CEntity oE = mActiveScene->CreateEntity(pName == "##Default" ? "Organism#" + std::to_string(mEIndex++) : pName);
		auto& trans = oE.AddComponent<CTransform2DComponent>();
		oE.AddComponent<CSpriteRendererComponent>().Texture=CircleTex;
		auto& organism = oE.AddComponent<COrganismComponent>();
		auto& energy = oE.AddComponent<CEnergyComponent>().Energy = organism.Data.InitialEnergy;
		auto& velocity = oE.AddComponent<CVelocityComponent>().Velocity = FVec2::ZeroVector();
		return oE;
	}

	float CLifeSim2D::HealthToSize(float pHealth)
	{
		return SMath::Max(pHealth*pHealth/8000, mMinSize);
	}

	float CLifeSim2D::LightToRadiation(float pLight)
	{
		return pLight*0.68;
	}

	CLifeSim2D::CWorldGeoData& CLifeSim2D::GetGeoDataFromLocation(FVec3 pLoc)
	{
		if (pLoc.x > mWorldMaxBounds.x || pLoc.x < -mWorldMaxBounds.x || pLoc.y > mWorldMaxBounds.y || pLoc.y < -mWorldMaxBounds.y) return mEmptySpace;

		float nearestX = float(std::floor((mWorldMaxBounds.x + pLoc.x) * mGridDensity));
		float nearestY = float(std::floor((mWorldMaxBounds.y - pLoc.y) * mGridDensity));
		
		return mWorldGeoData[int(nearestX + 2 * mWorldMaxBounds.x * mGridDensity * nearestY)];;
	}

	const CLifeSim2D::CWorldGeoData& CLifeSim2D::GetGeoDataFromLocation(FVec3 pLoc) const
	{
		if (pLoc.x > mWorldMaxBounds.x || pLoc.x < -mWorldMaxBounds.x || pLoc.y > mWorldMaxBounds.y || pLoc.y < -mWorldMaxBounds.y) return mEmptySpace;

		float nearestX = float(std::floor((mWorldMaxBounds.x + pLoc.x) * mGridDensity));
		float nearestY = float(std::floor((mWorldMaxBounds.y - pLoc.y) * mGridDensity));

		auto _ = int(nearestX + 2 * mWorldMaxBounds.x * mGridDensity * nearestY);

		return mWorldGeoData[_];
	}

	void CLifeSim2D::OnAttach(TRef<CScene> pActiveScene)
	{
		SetScene(pActiveScene);
		
		SSceneRegistry::SetupComponent<CEnergyComponent>();
		SSceneRegistry::SetupComponent<COrganismComponent>();
		SSceneRegistry::SetupComponent<CVelocityComponent>();

		{
		auto camE = mActiveScene->CreateEntity("Camera");
		auto& trans = camE.AddComponent<CTransform2DComponent>();
		auto& cam = camE.AddComponent<CCameraComponent>();
		cam.Primary = true;
		cam.Camera.SetOrthographicSize((mWorldMaxBounds.MaxComponent()*2.f));
		}

		for (int i = 0; i<1000; i++)
			MakeOrganism();

	}

	void CLifeSim2D::OnDetach()
	{
	}

	void CLifeSim2D::OnUpdateEditor(float pDeltaTime)
	{
		if (bPause) return;
		static const FVec3 Mask = FVec3(1, 1, 0);

	}

	void CLifeSim2D::OnUpdateRuntime(float pDeltaTime)
	{
		if (bPause) return;
		static const FVec3 Mask = FVec3(1, 1, 0);

		pDeltaTime*= mTimeMultiplier;
		mTime += pDeltaTime;

		std::set<CEntity> DeathRow;

		if (auto E = mActiveScene->GetPrimaryCameraEntity())
		{
			static bool itrlDO2 = true;
		
			SRenderer2D::BeginScene(E.GetComponent<CCameraComponent>().Camera, E.GetComponent<CTransform2DComponent>().Transform);
			SRenderer2D::DrawRect(FVec3::ZeroVector(), 0.f, mWorldMaxBounds * 2.f, FColor4::Yellow()*0.7f);
			SRenderer2D::DrawRect(FVec3::ZeroVector(), 0.f, (mWorldMaxBounds * 2.f) + 1, FColor4::Blue() * 0.9f);

			{
				auto view = mActiveScene->FilterByComponents<CTransform2DComponent, CVelocityComponent>();
				std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&](auto e) {
					auto& transformComponent = view.get<CTransform2DComponent>(e);
					auto& velocityComponent = view.get<CVelocityComponent>(e);

					(FVec2&)transformComponent.Transform.Location += velocityComponent.Velocity*pDeltaTime;
				});	
			}
			static float savedDT = 0;
			savedDT += pDeltaTime;
			{
				auto view= mActiveScene->FilterByComponents<CTransform2DComponent, COrganismComponent>();
				std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&](auto e) {
					auto& transformComponent = view.get<CTransform2DComponent>(e);
					auto& organism = view.get<COrganismComponent>(e);
					if (transformComponent.Transform.Location.x > mWorldMaxBounds.x) transformComponent.Transform.Location.x = -mWorldMaxBounds.x;
					if (transformComponent.Transform.Location.x < -mWorldMaxBounds.x) transformComponent.Transform.Location.x = mWorldMaxBounds.x;
					if (transformComponent.Transform.Location.y > mWorldMaxBounds.y) transformComponent.Transform.Location.y = -mWorldMaxBounds.y;
					if (transformComponent.Transform.Location.y < -mWorldMaxBounds.y) transformComponent.Transform.Location.y = mWorldMaxBounds.y;

					if (savedDT > SRandom::Float(1, 1.5)) {
						transformComponent.Transform.Scale = FVec2(HealthToSize(organism.Data.Health));

						auto E = CEntity(e, mActiveScene.get());
						auto& energy = E.GetComponent<CEnergyComponent>();
						float speed = 0.f;
						SRM::Convert(energy.Energy, speed, energy.Energy, mSpeedPerEnergy, organism.Data.Speed);

						E.GetComponent<CVelocityComponent>().Velocity = FVec2(SMath::Sin(SRandom::Float(-1, 1) * PIf), SMath::Cos(SRandom::Float(-1, 1) * PIf)) * speed;
						if (0.01f > energy.Energy) DeathRow.emplace(E);
					}
				});
			}
			if (savedDT>=1.2f) savedDT = 0.f;

			if (fmodf(mTime, 10.f) < 0.1f) {
				if (itrlDO2) {
					std::for_each(std::execution::par_unseq, mWorldGeoData, mWorldGeoData+mGridCount, [&](auto&& i) {
						auto lI = SRandom::Float(mMinLight, 5);
						i = CLifeSim2D::CWorldGeoData(lI);
					});
					itrlDO2 = false;
				}
			}
			else
				itrlDO2 = true;
// 			for (int i = 0; i < mGridCount; i++) {
// 				auto xScale = mWorldMaxBounds.x / (mWorldMaxBounds.x + mWorldMaxBounds.y);
// 				SRenderer2D::DrawQuad(
// 					FVec3(
// 						(i % int(mWorldMaxBounds.x * 2 * mGridDensity) - mWorldMaxBounds.x),
// 						xScale*(i / int(mWorldMaxBounds.y * 2 * mGridDensity)) - mWorldMaxBounds.y,
// 						-0.1),
// 
// 					0,
// 					FVec2(1, 1-xScale) / mGridDensity,
// 					ETransparencyType::Opaque,
// 					FColor4::Yellow() * mWorldGeoData[i].LightIntensity / 5);
// 			}

			auto view = mActiveScene->FilterByComponents<COrganismComponent, CEnergyComponent>();
			std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&](auto e) {
				// Photosynthesis

				auto& organism = view.get<COrganismComponent>(e);
				auto& energy = view.get<CEnergyComponent>(e);
				float photoAbsorb = GetLightIntensityAtLocation(E.GetComponent<CTransform2DComponent>().Transform.Location);
				energy.Energy = SMath::Clamp(energy.Energy + (photoAbsorb * organism.Data.PhotoSynthRate * pDeltaTime), 0.f, energy.MaxEnergy);
				// @TODO release appropriate energy amt
				//if (0.01f > energy.Energy) ARC_CORE_INFO("death {}", int(e)); DeathRow.emplace(E);
				
				//energy leak
				SRM::Consume(
					SMath::Interp<EaseOut<2>>(
						0.f,
						energy.MaxEnergy * mEnergyLeakMultiplier,
						organism.Data.Health / organism.Data.MaxHealth) * pDeltaTime,
					energy.Energy
				);
				//if (0.01f > energy.Energy) ARC_CORE_INFO("death {}", int(e)); DeathRow.emplace(E);
				
				// health regen
				SRM::Convert(
					energy.Energy,
					organism.Data.Health,
					energy.Energy * organism.Data.HealRate * pDeltaTime,
					mHealthPerEnergy,
					organism.Data.MaxHealth
				);

				//if (0.01f > energy.Energy) ARC_CORE_INFO("death {}", int(e)); DeathRow.emplace(E);
			});
			auto iter = mActiveScene->FilterByComponents<COrganismComponent>().each();

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
				auto view = mActiveScene->FilterByComponents<COrganismComponent, CEnergyComponent>();
				std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&](auto e) {
					// Photosynthesis

					auto& organism = view.get<COrganismComponent>(e);
					auto& energy = view.get<CEnergyComponent>(e);	auto E = CEntity(e, mActiveScene.get());
					if (energy.Energy == energy.MaxEnergy && energy.Energy >= mMinReproductionEnergyCost) // does not reproduce until max energy
					{
						// reproduce
						auto EChild = MakeOrganism(E.GetName()+"_child" + std::to_string(organism.NextChildsIndex++));
						auto& ec_organismComp = EChild.GetComponent<COrganismComponent>();
						auto& ec_energyComp = EChild.GetComponent<CEnergyComponent>();
					
						EChild.GetComponent<CTransform2DComponent>().Transform.Location = E.GetComponent<CTransform2DComponent>().Transform.Location;
						EChild.GetComponent<CTransform2DComponent>().Transform.Location.y += E.GetComponent<CTransform2DComponent>().Transform.Scale.MaxComponent() + EChild.GetComponent<CTransform2DComponent>().Transform.Scale.MaxComponent() + 0.1f;

						float RadiationFactor = LightToRadiation(GetLightIntensityAtLocation(E.GetComponent<CTransform2DComponent>().Transform.Location));

						ec_organismComp.Data.MaxHealth += SRandom::Float(-ec_organismComp.MutationData.MaxHealth, ec_organismComp.MutationData.MaxHealth)*RadiationFactor;
						ec_organismComp.Data.Speed += SRandom::Float(-ec_organismComp.MutationData.Speed, ec_organismComp.MutationData.Speed) * RadiationFactor;
						ec_organismComp.Data.PhotoSynthRate += SRandom::Float(-ec_organismComp.MutationData.PhotoSynthRate, ec_organismComp.MutationData.PhotoSynthRate) * RadiationFactor;
						ec_organismComp.Data.HealRate += SRandom::Float(-ec_organismComp.MutationData.HealRate, ec_organismComp.MutationData.HealRate) * RadiationFactor;
						ec_organismComp.Data.InitialEnergy += SRandom::Float(-ec_organismComp.MutationData.InitialEnergy, ec_organismComp.MutationData.InitialEnergy) * RadiationFactor;
						ec_organismComp.Data.PostReproductionEnergyReserve += SRandom::Float(-ec_organismComp.MutationData.PostReproductionEnergyReserve, ec_organismComp.MutationData.PostReproductionEnergyReserve) * RadiationFactor;
						ec_energyComp.MaxEnergy += SRandom::Float(-ec_organismComp.MutationData.MaxEnergy, ec_organismComp.MutationData.MaxEnergy) * RadiationFactor;

						ec_organismComp.Data.MaxHealth = SMath::Max(0.f, ec_organismComp.Data.MaxHealth);
						ec_organismComp.Data.Speed = SMath::Max(0.f, ec_organismComp.Data.Speed);
						ec_organismComp.Data.PhotoSynthRate = SMath::Max(0.f, ec_organismComp.Data.PhotoSynthRate);
						ec_organismComp.Data.HealRate = SMath::Max(0.f, ec_organismComp.Data.HealRate);
						ec_organismComp.Data.InitialEnergy = SMath::Max(0.f, ec_organismComp.Data.InitialEnergy);
						ec_organismComp.Data.PostReproductionEnergyReserve = SMath::Max(0.f, ec_organismComp.Data.PostReproductionEnergyReserve);
						ec_energyComp.MaxEnergy = SMath::Max(0.f, ec_energyComp.MaxEnergy);

						ec_organismComp.Data.Health = 0.f;

						SRM::Consume(mMinReproductionEnergyCost, energy.Energy, organism.Data.PostReproductionEnergyReserve);
						SRM::Consume(ec_organismComp.Data.InitialEnergy, energy.Energy, organism.Data.PostReproductionEnergyReserve);
						SRM::Convert(energy.Energy, ec_organismComp.Data.Health, energy.Energy - organism.Data.PostReproductionEnergyReserve, mHealthPerEnergy, ec_organismComp.Data.MaxHealth);
					}
					if (0.01f > energy.Energy) { 
						ARC_CORE_INFO("reproduction death{}", int(e)); DeathRow.emplace(E);
					}
				});
			}

			for (auto i : DeathRow) {
				i.Kill();
			}


			SRenderer2D::EndScene();
		}
	}

	void CLifeSim2D::OnGuiRender()
	{
		ImGui::Begin("LS2D");

		static FVec3 tmp = FVec3::ZeroVector();
		float tmp2 = LightToRadiation(GetLightIntensityAtLocation(tmp)) / 5;
		float tmp3 = GetLightIntensityAtLocation(tmp);
		ImGui::DragFloat3("Rad@Loc", tmp.Data());
		ImGui::DragFloat("Light@Loc", &tmp3, ImGuiSliderFlags_ReadOnly);
		ImGui::DragFloat("LightTmp", &tmp2, ImGuiSliderFlags_ReadOnly);
		
		ImGui::DragFloat("Time Multiplier", &mTimeMultiplier);
		ImGui::DragFloat("World Time", &mTime);
		float abcd = TimeToTimeOfDay();
		ImGui::DragFloat("Time of day", &abcd);
		abcd = GetLightIntensity();
		ImGui::DragFloat("LightIntensity", &abcd);
		abcd = LightToRadiation(GetLightIntensity());
		ImGui::DragFloat("Radiation", &abcd);

		TString timeofday = abcd > 0.2f ? "Morning" : "Night";

		ImGui::Text(timeofday.c_str());

		ImGui::End();
	}

	void CLifeSim2D::OnEvent(CEvent& pEvent)
	{
	}

	void CLifeSim2D::SetScene(TRef<CScene> pScene)
	{
		mActiveScene = pScene;
		mTime = 0.f;
		delete[] mWorldGeoData;
		mGridCount = int(4.f * mWorldMaxBounds.x* (mWorldMaxBounds.y+0.5) * mGridDensity)+1;
		mWorldGeoData = new CWorldGeoData[mGridCount];

		for (int i = 0; i < mGridCount; i++)
			mWorldGeoData[i] = CLifeSim2D::CWorldGeoData(SRandom::Float(mMinLight, 5));

	}

	float CLifeSim2D::TimeToTimeOfDay() const
	{
		return fmodf(mTime, mDayLength);
	}

	float CLifeSim2D::GetLightIntensity() const
	{
		auto _ = SMath::Clamp(
			(SMath::Sin(SMath::Interp<Linear>(0.f, 1.2f * PIf, TimeToTimeOfDay() / mDayLength))) * mLightMultiplier, 
			mMinLight,
			mLightMultiplier
		);
		return _;
	}

	float CLifeSim2D::GetLightIntensityAtLocation(FVec3 pLoc) const
	{
		return GetLightIntensity()* GetGeoDataFromLocation(pLoc).LightIntensity;
	}
}