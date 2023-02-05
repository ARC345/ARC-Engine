#include "PCH/arc_pch.h"
#include "Atom.h"
#include "ARC/Scene/Scene.h"
#include "imgui/imgui.h"
#include "ARC/Scene/Entity.h"
#include "ARC/Scene/BasicComponents.h"
#include "ARC/Scene/SceneRegistry.h"
#include "ARC/Helpers/Random.h"

namespace ARC {
	// 2d only
	static float Time_Multiplier = 1.f;
	static float TimeElapsed_World = 0.0;
	static float TimeElapsed_ThisFrame = 0.0;
	static bool bPause = false;

	void CElectricSignComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		static const char* SignStrings[] = { "Negative", "Neutral", "Positive" };
		const char* currentSignString = SignStrings[1 + Sign];

		if (ImGui::BeginCombo("Sign", currentSignString))
		{
			for (int i = 0; i < 3; i++)
			{
				bool isSelected = currentSignString == SignStrings[i];
				if (ImGui::Selectable(SignStrings[i], isSelected))
				{
					currentSignString = SignStrings[i];
					Sign = i - 1;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

	}

	void CElectricSignComponent::Serialize(YAML::Emitter& pOut)
	{
		//pOut << YAML::Key << "Sign" << YAML::Value << Sign;
	}

	void CElectricSignComponent::Deserialize(YAML::Node& pData)
	{
		//Sign = pData["Sign"].as<int8_t>();
	}

	static FVec3 FindClosestPointOnLine(FVec3 pLB, FVec3 pLE, FVec3 pP)
	{
		float LineLength = SMath::Dist(pLB, pLE);
		FVec3 Vector = pP - pLB;
		FVec3 LineDirection = (pLE - pLB) / LineLength;

		FVec3 tmp(Vector * LineDirection);
		float Distance = tmp.x+tmp.y;
		
		if (Distance <= float(0)) return pLB;
		if (Distance >= LineLength) return pLE;
		return pLB + LineDirection * Distance;
	}

	void CAtomExp::OnAttach(TRef<CScene>& pActiveScene)
	{
		SSceneRegistry::SetupComponent<CNetForceComponent>();
		SSceneRegistry::SetupComponent<CElectricSignComponent>();

		m_ActiveScene = pActiveScene;
		auto cam = m_ActiveScene->CreateEntity("Camera");
		auto& ccc = cam.AddComponent<CCameraComponent>();
		ccc.bPrimary = true;
		auto& camTransformComp = cam.AddComponent<CTransform2DComponent>();
		
		auto tex = CTexture2D::Create("assets/textures/circle-64.png");
		auto trans = FTransform2D();

		{
		auto electron = m_ActiveScene->CreateEntity("Atom1");
		auto& _ = electron.AddComponent<CTransform2DComponent>(trans);
		_.Transform.Location = FVec3(SRandom::Float(-10, 10), SRandom::Float(-10, 10), 0);
		electron.AddComponent<CSpriteRendererComponent>(FColor4::Blue(), tex, FVec2::OneVector());
		}{
		auto electron = m_ActiveScene->CreateEntity("Atom2");
		auto& _ = electron.AddComponent<CTransform2DComponent>(trans);
		_.Transform.Location = FVec3(SRandom::Float(-10, 10), SRandom::Float(-10, 10), 0);
		electron.AddComponent<CSpriteRendererComponent>(FColor4::Blue(), tex, FVec2::OneVector());
		}
		for (size_t i = 0; i < 100; i++)
		{
		}
	}

	void CAtomExp::OnDetach()
	{
	}

	void CAtomExp::OnUpdate(float pDeltaTime)
	{
		if (bPause) return;
		static const FVec3 Mask = FVec3(1, 1, 0);

		TimeElapsed_ThisFrame = pDeltaTime * Time_Multiplier;
		TimeElapsed_World += TimeElapsed_ThisFrame;

		// 
		{
			auto filterC = m_ActiveScene->FilterByComponents<CMassComponent, CSpriteRendererComponent, CElectricSignComponent>();

			for (auto [eid, cmc, csrc, cesn] : filterC.each())
			{
				if (cesn.Sign == CElectricSignComponent::EElectricSign::Negative)
				{
					cmc.Mass = 9.10938e-31f;
					csrc.Color = FColor4::Blue();
				}
				else if (cesn.Sign == CElectricSignComponent::EElectricSign::Neutral)
				{
					cmc.Mass = 1.6749e-27f;
					csrc.Color = FColor4::Yellow();
				}
				else if (cesn.Sign == CElectricSignComponent::EElectricSign::Positive)
				{
					cmc.Mass = 1.67262192e-27f;
					csrc.Color = FColor4::Red();
				}
			}
		}


		// Gravity
		{
			auto filterC = m_ActiveScene->FilterByComponents<CNetForceComponent, CTransform2DComponent, CMassComponent>();

			static constexpr double gravityConstant = 6.6743e-11f; // m3 kg-1 s-2

			for (auto&& [eid, cnfc, ctc, cmc] : filterC.each())
				for (auto&& [eid1, cnfc1, ctc1, cmc1] : filterC.each())
					if (eid != eid1)
					{
						auto ctc1_to_ctc = ctc1.Transform.Location - ctc.Transform.Location;
						FVec3 force = ctc1_to_ctc.Normalize() * gravityConstant * cmc.Mass * cmc1.Mass / SMath::DistSqr(ctc.Transform.Location, ctc1.Transform.Location);
						cnfc.NetForce += force;
					}
		}
		// Coulomb force
		{
			auto filterC = m_ActiveScene->FilterByComponents<CNetForceComponent, CTransform2DComponent, CElectricSignComponent>();

			static constexpr double coulombConstant = 8.9e9; // N / m2
			static constexpr double chargeInProton = 1.602e-19;

			for (auto&& [eid, cnfc, ctc, cesc] : filterC.each())
				for (auto&& [eid1, cnfc1, ctc1, cesc1] : filterC.each())
					if (eid != eid1)
					{
						auto UnitVector = (ctc1.Transform.Location - ctc.Transform.Location).Normalize();
						if (auto dist = SMath::DistSqr(ctc.Transform.Location, ctc1.Transform.Location))
						{
							FVec3 force = UnitVector * -1.f * chargeInProton * chargeInProton * coulombConstant * float(cesc.Sign) * float(cesc1.Sign) / dist;
							cnfc.NetForce += force;
						}
					}
		}

		// Overlap Response
		{
			auto filterC = m_ActiveScene->FilterByComponents<CNetForceComponent, CTransform2DComponent, CMassComponent, CVelocityComponent>();
			
			for (auto&& [eid, cnfc, ctc, cmc, cvc] : filterC.each())
			{
				FVec3 acc = cnfc.NetForce / cmc.Mass;
				cnfc.NetForce = FVec3::ZeroVector();
				float rad = ctc.Transform.Scale.x / 2;
				cvc.Velocity += acc * pDeltaTime * Time_Multiplier;
				cvc.Velocity *= Mask;
			}
		}

		// Collision Response
		{
			auto filterC = m_ActiveScene->FilterByComponents<CTransform2DComponent, CMassComponent, CVelocityComponent>();
			for (auto&& [eid, ctc, cmc, cvc] : filterC.each())
			{
				const FVec3& CurrLoc = ctc.Transform.Location * Mask;
				float rad = ctc.Transform.Scale.x / 2;

				for (auto&& [eid1, ctc1, cmc1, cvc1] : filterC.each())
				{
					const FVec3& CurrLoc1 = ctc1.Transform.Location * Mask;

					if (eid != eid1)
					{
						float dist = SMath::DistSqr(CurrLoc*Mask, CurrLoc1 * Mask);
						float rad1 = ctc1.Transform.Scale.x / 2;
						if (dist <= SMath::Sqr(ctc.Transform.Scale.x + ctc1.Transform.Scale.x)/4)
						{
							ARC_CORE_INFO("Collision");
							float overlap = 0.5f * (dist - rad - rad1);

							ctc.Transform.Location -= (CurrLoc - CurrLoc1) * Mask * overlap / dist;
							ctc1.Transform.Location += (CurrLoc - CurrLoc1) * Mask * overlap / dist;

							FVec3 normal = (CurrLoc1 - CurrLoc) / dist;
							FVec3 k = cvc.Velocity-cvc1.Velocity;

							float p = 2.0 * (normal.x * k.x + normal.y * k.y) / (cmc.Mass+cmc1.Mass);
							cvc.Velocity -= normal*p*cmc1.Mass*pDeltaTime * Time_Multiplier;
							cvc1.Velocity += normal*p*cmc1.Mass*pDeltaTime*Time_Multiplier;
							cvc.Velocity *= Mask;
							cvc1.Velocity *= Mask;
						}
					}
				}
			}
		}
	}

	void CAtomExp::OnGuiRender()
	{
		ImGui::Begin("Atomic");
		ImGui::DragFloat("Speed", &Time_Multiplier);
		ImGui::DragFloat("Time Elapsed(World)", &TimeElapsed_World);
		ImGui::Checkbox("Pause", &bPause);
		ImGui::End();
	}

	void CAtomExp::OnEvent(CEvent& pEvent)
	{
	}
}