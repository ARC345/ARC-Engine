#include "arc_pch.h"
#include "Scene.h"
#include "ARC\Renderer\Renderer2D.h"
#include "ARC\Objects\Primitive2D.h"

namespace ARC {
	CScene::CScene()
	{
		CEntity entity(m_Manager.CreateIndex(), this);
		entity.AddTag<InvalidEntityTag>();
	}

	CScene::~CScene()
	{

	}

	CEntity CScene::CreateEntity(const TString& pName)
	{
		CEntity entity(m_Manager.CreateIndex(), this);
		entity.AddComponent<CTransform2DComponent>();
		entity.AddComponent<CNameComponent>(pName);

		return entity;
	}

	void CScene::OnUpdate(float DeltaTime)
	{
		{
			auto& comps = m_Manager.GetAllComponentsOfClass<CNativeScriptComponent>();
			for (auto& comp : comps)
			{
				if (!comp.Controller)
				{
					comp.Controller = comp.InstantiateDelegate();
					comp.Controller->Setup(comp.OwningEntity);
					comp.Controller->OnCreate();
				}

				comp.Controller->OnUpdate(DeltaTime);
			}
		}
		CCamera* MainCam = nullptr;
		FTransform2D* CamTransform = nullptr;
		{
			std::vector<EntityID> OutEntities = m_Manager.GetMatchingEntities<CTransform2DComponent, CCameraComponent>();
			for (auto entity : OutEntities)
			{
				auto& [TransformComp, CameraComp] = m_Manager.GetComponents<CTransform2DComponent, CCameraComponent>(entity);
				if (CameraComp.bPrimary)
				{
					MainCam = &CameraComp.Camera;
					CamTransform = &TransformComp.Transform;
					break;
				}
			}
		}
		if (MainCam)
		{
			CRenderer2D::BeginScene(*MainCam, *CamTransform);

			std::vector<EntityID> OutEntities = m_Manager.GetMatchingEntities<CTransform2DComponent, CSpriteRendererComponent>();

			CPrimitive2D Quad;
			for (auto entity : OutEntities)
			{
				auto& [TransformComp, SpriteRenderComp] = m_Manager.GetComponents<CTransform2DComponent, CSpriteRendererComponent>(entity);
				Quad.Color = SpriteRenderComp;
				Quad.Transform = TransformComp;
				CRenderer2D::DrawQuad(Quad);
			}

			CRenderer2D::EndScene();
		}
		m_Manager.Refresh();
	}

	void CScene::OnViewportResize(TVec2<uint32_t> pNewSize)
	{
		m_ViewportSize = pNewSize;

		auto& Comps = m_Manager.GetAllComponentsOfClass<CCameraComponent>();

		for (auto& component : Comps)
		{
			if (!component.bFixedAspectRatio)
			{
				component.Camera.SetViewportSize(pNewSize);
			}
		}
	}

}