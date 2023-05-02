#pragma once
#include "ARC/Renderer/Layer.h"
#include "ARC/Scene/Component.h"

namespace ARC { class CScene; }
namespace ARC { class CEvent; }

namespace ARC
{
	struct CEnergyComponent : public CComponentBase
	{
		float Energy = 10.f;
		float MaxEnergy = 100.f;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};

	struct COrganismComponent : public CComponentBase
	{
		struct COrganismData {
			float MaxHealth = 100;
			float Health = MaxHealth; // energy leak multiplier = 1 - health
			float Speed = 1.f; // defines how fast can organism move// acceleration consumes energy// unused
			float PhotoSynthRate = 0.69;
			float HealRate = 1;

			// @TODO gui
			float PostReproductionEnergyReserve = 20.f;
			float InitialEnergy = 10;
		};

		struct COrganismMutationData {
			float MaxHealth = 1;
			float Speed = 0.1;
			float PhotoSynthRate = 0.1;
			float HealRate = 0.1;
			float PostReproductionEnergyReserve = 1;
			float InitialEnergy = 1;
			float MaxEnergy = 1;

			// @TODO
			//COrganismData InitialDataMultiplier= COrganismData(0.2f, 0.2f, 1.2f, 1.5f, 0.2f, 1.f); 
			//float TimeTillFullGrowth = 2.f; // in days
		};

		COrganismData Data;
		COrganismMutationData MutationData;

		TUInt32 NextChildsIndex = 0; // next name of child && reward

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};

	struct CVelocityComponent : public CComponentBase { 
		FVec2 Velocity;
		virtual TUInt32 GetFlags() override { return Flags; };
		static inline TUInt32 Flags = 0;
	};

	class CLifeSim2D
	{

	public:
		void OnAttach(TRef<CScene> pActiveScene);
		void OnDetach();
		void OnUpdateEditor(float pDeltaTime);
		void OnUpdateRuntime(float pDeltaTime);
		void OnGuiRender();
		void OnEvent(CEvent& pEvent);

		void SetScene(TRef<CScene> pScene);

		float TimeToTimeOfDay() const;
		float GetLightIntensity() const;
		float GetLightIntensityAtLocation(FVec3 pLoc) const;

		CEntity MakeOrganism(const TString& pName = "##Default");

		float HealthToSize(float pHealth);
		float LightToRadiation(float pLight);
		struct CWorldGeoData { float LightIntensity = 1.f; };

		CWorldGeoData& GetGeoDataFromLocation(FVec3 pLoc);
		const CWorldGeoData& GetGeoDataFromLocation(FVec3 pLoc) const;

	private:
		static inline CWorldGeoData mEmptySpace = CWorldGeoData(0.f);
		CWorldGeoData* mWorldGeoData;
		int mGridDensity = 1; // grid per unit area
		int mGridCount = 0; // grid per unit area
		FVec2 mWorldMaxBounds= FVec2(60.f, 40.f);

		float mTime = 0.f;
		float mTimeMultiplier = 1.f;
		float mDayLength = 100.f; // length of 1 day
		float mLightMultiplier = 1.f;
		float mMinLight = 0.01;
		float mEnergyLeakMultiplier = 0.01f;
		float mHealthPerEnergy = 2.f;
		float mMinReproductionEnergyCost = 15.f;
		float mSpeedPerEnergy = 10.f;
		float mMinSize = 0.35f;

		TUInt32 mEIndex = 0;

		TRef<CScene> mActiveScene;
	};
}
