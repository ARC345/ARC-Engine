#include "PCH/arc_pch.h"
#include "LifeSim2D.h"
#include "ARC/Scene/Scene.h"
#include "imgui/imgui.h"
#include "ARC/Scene/Entity.h"
#include "ARC/Scene/BasicComponents.h"
#include "ARC/Scene/ColliderComponents.h"
#include "ARC/Scene/SceneRegistry.h"
#include "ARC/Helpers/Random.h"

namespace ARC {
	// 2d only
	static float Time_Multiplier = 1.f;
	static float TimeElapsed_World = 0.0;
	static float TimeElapsed_ThisFrame = 0.0;
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

	void CLifeSim2D::OnAttach(TRef<CScene>& pActiveScene)
	{
		SSceneRegistry::SetupComponent<CNetForceComponent>();

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
			_.Transform.Location = FVec3(-10, 0, 0);
			electron.AddComponent<CSpriteRendererComponent>(FColor4::Yellow(), tex, FVec2::OneVector());
			electron.AddComponent<CVelocityComponent>(FVec3(0.01f,0.f,0.f));
			electron.AddComponent<CMassComponent>();
			electron.AddComponent<CCircleColliderComponent>(FVec3::ZeroVector(), 2.f);
		} {
			auto electron = m_ActiveScene->CreateEntity("Atom2");
			auto& _ = electron.AddComponent<CTransform2DComponent>(trans);
			_.Transform.Location = FVec3(10, 0, 0);
			electron.AddComponent<CSpriteRendererComponent>(FColor4::Blue(), tex, FVec2::OneVector());
			electron.AddComponent<CVelocityComponent>(FVec3(-0.01f, 0.f, 0.f));
			electron.AddComponent<CMassComponent>();
			electron.AddComponent<CCircleColliderComponent>(FVec3::ZeroVector(), 1.25f);
		}
	}

	void CLifeSim2D::OnDetach()
	{
	}

	void CLifeSim2D::OnUpdate(float pDeltaTime)
	{
		if (bPause) return;
		static const FVec3 Mask = FVec3(1, 1, 0);

		TimeElapsed_ThisFrame = pDeltaTime * Time_Multiplier;
		TimeElapsed_World += TimeElapsed_ThisFrame;
		//
		//
		//// Gravity
		//{
		//	auto filterC = m_ActiveScene->FilterByComponents<CNetForceComponent, CTransform2DComponent, CMassComponent>();
		//
		//	static constexpr double gravityConstant = 6.6743e-11f; // m3 kg-1 s-2
		//
		//	for (auto&& [eid, cnfc, ctc, cmc] : filterC.each())
		//		for (auto&& [eid1, cnfc1, ctc1, cmc1] : filterC.each())
		//			if (eid != eid1)
		//			{
		//				auto ctc1_to_ctc = ctc1.Transform.Location - ctc.Transform.Location;
		//				FVec3 force = ctc1_to_ctc.Normalize() * gravityConstant * cmc.Mass * cmc1.Mass / SMath::DistSqr(ctc.Transform.Location, ctc1.Transform.Location);
		//				cnfc.NetForce += force;
		//			}
		//}
		//
		
		// Overlap Response
		{
			auto filterC = m_ActiveScene->FilterByComponents<CNetForceComponent, CMassComponent, CVelocityComponent>();

			for (auto&& [eid, cnfc, cmc, cvc] : filterC.each())
			{
				FVec3 acc = cnfc.NetForce / cmc.Mass;
				cnfc.NetForce = FVec3::ZeroVector();
				cvc.Velocity += acc * pDeltaTime * Time_Multiplier;
				cvc.Velocity *= Mask;
			}
		}

		{
			auto filterC = m_ActiveScene->FilterByComponents<CCircleColliderComponent, CTransform2DComponent>();

			for (auto&& [e0_id, e0_ccc, e0_ctc] : filterC.each())
			{
				FVec3 CurrLoc = (e0_ctc.Transform.Location + e0_ccc.Location) * Mask;

				for (auto&& [e1_id, e1_ccc, e1_ctc] : filterC.each())
				{
					if (e0_id != e1_id)
					{
						FVec3 CurrLoc1 = (e1_ctc.Transform.Location + e1_ccc.Location) * Mask;

						float dist = SMath::DistSqr(CurrLoc, CurrLoc1);
						
						if (dist > SMath::Sqr(e0_ccc.Radius + e1_ccc.Radius)) continue;
						
						ARC_CORE_INFO("Collision");
						dist = SMath::Sqrt(dist);
						float overlap = 0.5f * (dist - e0_ccc.Radius - e1_ccc.Radius);

						e0_ctc.Transform.Location -= (CurrLoc - CurrLoc1) * overlap / dist;
						e1_ctc.Transform.Location += (CurrLoc - CurrLoc1) * overlap / dist;

						auto e0 = CEntity(e0_id, m_ActiveScene.get());
						auto e1 = CEntity(e1_id, m_ActiveScene.get());

						if (!(e0.HasComponent<CMassComponent>() && e1.HasComponent<CMassComponent>())) continue;

						FVec3* CurrVel0 = e0.HasComponent<CVelocityComponent>() ? &e0.GetComponent<CVelocityComponent>().Velocity : nullptr;
						FVec3* CurrVel1 = e1.HasComponent<CVelocityComponent>() ? &e1.GetComponent<CVelocityComponent>().Velocity : nullptr;

						float CurrMass0 = CurrVel0 ?e0.GetComponent<CMassComponent>().Mass : 100000000;
						float CurrMass1 = CurrVel1 ?e1.GetComponent<CMassComponent>().Mass : 100000000;

						FVec3 normal = (CurrLoc1 - CurrLoc) / dist;
						FVec3 k = (CurrVel0 ? *CurrVel0 : FVec3::ZeroVector()) - (CurrVel1 ? *CurrVel1 : FVec3::ZeroVector());

						float p = 2.0 * (normal.x * k.x + normal.y * k.y) / (CurrMass0 + CurrMass1);

						if (CurrVel0)
							*CurrVel0 -= normal * p * CurrMass0 * pDeltaTime * Time_Multiplier;
						if (CurrVel1)
							*CurrVel1 += normal * p * CurrMass1 * pDeltaTime * Time_Multiplier;
					}
				}
			}
		}
	}

	void CLifeSim2D::OnGuiRender()
	{
		ImGui::Begin("Atomic");
		ImGui::DragFloat("Speed", &Time_Multiplier);
		ImGui::DragFloat("Time Elapsed(World)", &TimeElapsed_World);
		ImGui::Checkbox("Pause", &bPause);
		ImGui::End();
	}

	void CLifeSim2D::OnEvent(CEvent& pEvent)
	{
	}
}