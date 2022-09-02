#include "PCH/arc_pch.h"
#include "Atom.h"
#include "ARC/Scene/Scene.h"
#include "imgui/imgui.h"

namespace ARC {
	// 2d only
	static float Time_Multiplier = 1.f;
	static float TimeElapsed_World = 0.0;
	static float TimeElapsed_ThisFrame = 0.0;
	static bool bPause = 0;

	static FVec3 FindClosestPointOnLine(FVec3 pLB, FVec3 pLE, FVec3 pP)
	{
		float LineLength = Math::Dist(pLB, pLE);
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
		m_ActiveScene = pActiveScene;
		auto cam = m_ActiveScene->CreateEntity("Camera");
		auto& ccc = cam.AddComponent<CCameraComponent>();
		ccc.bPrimary = true;

		auto tex = CTexture2D::Create("assets/textures/circle-64.png");

		auto electron = m_ActiveScene->CreateEntity("Atom");
		electron.GetComponent<CTransform2DComponent>().Transform.Location = FVec3::OneVector;
		electron.AddComponent<CSpriteRendererComponent>(FColor::Blue, tex, FVec2::OneVector);
		auto electron2 = m_ActiveScene->CreateEntity("Atom2");
		electron2.AddComponent<CSpriteRendererComponent>(FColor::Blue, tex, FVec2::OneVector);
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
			auto& filterC = m_ActiveScene->GetManager().FilterComponents<CMassComponent, CSpriteRendererComponent, CElectricSignComponent>();

			for (auto [eid, cmc, csrc, cesn] : filterC)
			{
				if (cesn.Sign == CElectricSignComponent::EElectricSign::Negative)
				{
					cmc.Mass = 9.10938e-31;
					csrc.Color = FColor::Blue;
				}
				else if (cesn.Sign == CElectricSignComponent::EElectricSign::Neutral)
				{
					cmc.Mass = 1.6749e-27;
					csrc.Color = FColor::Yellow;
				}
				else if (cesn.Sign == CElectricSignComponent::EElectricSign::Positive)
				{
					cmc.Mass = 1.67262192e-27;
					csrc.Color = FColor::Red;
				}
			}
		}


		// Gravity
		{
			auto& filterC = m_ActiveScene->GetManager().FilterComponents<CNetForceComponent, CTransform2DComponent, CMassComponent>();

			static constexpr double gravityConstant = 6.6743e-11; // m3 kg-1 s-2

			for (auto [eid, cnfc, ctc, cmc] : filterC)
				for (auto [eid1, cnfc1, ctc1, cmc1] : filterC)
					if (eid != eid1)
					{
						auto ctc1_to_ctc = ctc1.Transform.Location - ctc.Transform.Location;
						FVec3 force = ctc1_to_ctc.Normalize() * gravityConstant * cmc.Mass * cmc1.Mass / Math::DistSqr(ctc.Transform.Location, ctc1.Transform.Location);
						cnfc.NetForce += force;
					}
		}
		// Coulomb force
		{
			auto& filterC = m_ActiveScene->GetManager().FilterComponents<CNetForceComponent, CTransform2DComponent, CElectricSignComponent>();

			static constexpr double coulombConstant = 8.9e9; // N / m2
			static constexpr double chargeInProton = 1.602e-19;

			for (auto [eid, cnfc, ctc, cesc] : filterC)
				for (auto [eid1, cnfc1, ctc1, cesc1] : filterC)
					if (eid != eid1)
					{
						auto UnitVector = (ctc1.Transform.Location - ctc.Transform.Location).Normalize();
						if (auto dist = Math::DistSqr(ctc.Transform.Location, ctc1.Transform.Location))
						{
							FVec3 force = UnitVector * -1.f * chargeInProton * chargeInProton * coulombConstant * float(cesc.Sign) * float(cesc1.Sign) / dist;
							cnfc.NetForce += force;
						}
					}
		}

		// Overlap Response
		{
			auto& filterC = m_ActiveScene->GetManager().FilterComponents<CNetForceComponent, CTransform2DComponent, CMassComponent, CVelocityComponent>();
			
			for (auto [eid, cnfc, ctc, cmc, cvc] : filterC)
			{
				FVec3 acc = cnfc.NetForce / cmc.Mass;
				cnfc.NetForce = FVec3::ZeroVector;
				float rad = ctc.Transform.Scale.x / 2;
				cvc.Velocity += acc * pDeltaTime * Time_Multiplier;
				cvc.Velocity *= Mask;
			}
		}

		// Collision Response
		{
			auto& filterC = m_ActiveScene->GetManager().FilterComponents<CTransform2DComponent, CMassComponent, CVelocityComponent>();
			for (auto [eid, ctc, cmc, cvc] : filterC)
			{
				const FVec3& CurrLoc = ctc.Transform.Location * Mask;
				float rad = ctc.Transform.Scale.x / 2;

				for (auto [eid1, ctc1, cmc1, cvc1] : filterC)
				{
					const FVec3& CurrLoc1 = ctc1.Transform.Location * Mask;

					if (eid != eid1)
					{
						float dist = Math::DistSqr(CurrLoc*Mask, CurrLoc1 * Mask);
						float rad1 = ctc1.Transform.Scale.x / 2;
						if (dist <= Math::Sqr(ctc.Transform.Scale.x + ctc1.Transform.Scale.x)/4)
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