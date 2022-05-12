#include "arc_pch.h"
#include "Scene.h"
#include "ARC\Renderer\Renderer2D.h"
#include "ARC\Objects\Primitive2D.h"
#include "Component.h"

namespace ARC {
	CScene::CScene()
	{
		// entity 0 is invalid
		CEntity entity(m_Manager.CreateEntity(), this);
		//entity.AddTag<InvalidEntityTag>();

		SetupComponents();
	}

	CScene::~CScene()
	{

	}

	void CScene::SetupComponents()
	{
	}

	CEntity CScene::CreateEntity(const TString& pName)
	{
		CEntity entity(m_Manager.CreateEntity(), this);
		entity.AddComponent<CTransform2DComponent>();
		entity.AddComponent<CNameComponent>(pName);

		return entity;
	}

	void CScene::RemoveEntity(CEntity Entity)
	{
		m_Manager.Kill(Entity.GetID());
		Entity.OnKill();
	}

	void CScene::OnUpdate(float DeltaTime)
	{
		//m_Manager.Refresh();
		{
			auto& comps = m_Manager.GetComponents<CNativeScriptComponent>();
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
			auto filter = m_Manager.FilterComponents<CTransform2DComponent, CCameraComponent>();
			for (auto[entity, TransformComp, CameraComp] : filter)
			{
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

			auto filter = m_Manager.FilterComponents<CTransform2DComponent, CSpriteRendererComponent>();
			auto filter2 = m_Manager.GetComponents<CCameraComponent>();
			
			if (filter2.size()!=1)
			{
				static CPrimitive2D Quad;
				CRenderer2D::DrawQuad(Quad);
			}

			for (auto [entity, TransformComp, SpriteRendererComp] : filter)
			{
				static CPrimitive2D Quad;
				Quad.Color = SpriteRendererComp;
				Quad.Transform = TransformComp;
				CRenderer2D::DrawQuad(Quad);
			}

			CRenderer2D::EndScene();
		}
	}

	void CScene::OnViewportResize(TVec2<uint32_t> pNewSize)
	{
		m_ViewportSize = pNewSize;

		auto& Comps = m_Manager.GetComponents<CCameraComponent>();

		for (auto& component : Comps)
		{
			if (!component.bFixedAspectRatio)
			{
				component.Camera.SetViewportSize(pNewSize);
			}
		}
	}

}