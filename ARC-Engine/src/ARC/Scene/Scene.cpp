#include "arc_pch.h"
#include "Scene.h"
#include "ARC\Renderer\Renderer2D.h"
#include "ARC\Objects\Primitive2D.h"
#include "Utils\MPL\Interface.hpp"
#include "Utils\MetaFor.hpp"

namespace ARC {
	CScene::CScene()
	{
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

		MPL::forTypes<MyComponents>([&](auto t){
			using tCompType = ECS_TYPE(t);
			if constexpr (tCompType::Flags & ECF::AutoAddToAll)
			{
				if constexpr (std::is_same_v<tCompType, CNameComponent>)
					entity.AddComponent<CNameComponent>(pName);
				else
					entity.AddComponent<tCompType>();
			}
		});

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
				Quad.Color = SpriteRendererComp.Color;
				Quad.Texture = SpriteRendererComp.Texture;
				Quad.TextureScaling = SpriteRendererComp.TextureScaling;
				Quad.Transform = TransformComp;
				CRenderer2D::DrawQuad(Quad);
			}

			CRenderer2D::EndScene();
		}

		{
			auto filter = m_Manager.FilterComponents<CTransform2DComponent, CVelocityComponent>();
			for (auto [entity, ctc, cvc] : filter)
				ctc.Transform.Location += cvc.Velocity;
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

	void CScene::SerializeEntity(YAML::Emitter& pOut, CEntity pEntity)
	{
		pOut << YAML::BeginMap;
		pOut << YAML::Key << "Entity" << YAML::Value << "1038845309209";

		MPL::forTypes<MyComponents>([&](auto t) {
			using tCompType = ECS_TYPE(t);

				if (pEntity.HasComponent<tCompType>())
				{
					pOut << YAML::Key << CComponentTraits::GetName<tCompType>();
					pOut << YAML::Value << YAML::BeginMap;
					if constexpr (tCompType::Flags & ECF::Serializable)
						pEntity.GetComponent<tCompType>().Serialize(pOut);
					pOut << YAML::EndMap;
				}

			});

		pOut << YAML::EndMap;
	}

	void CScene::SerializeToText(const TString& pFilepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Manager.UpdateEntities([&](auto pEid){
			CEntity entity = { pEid, this };
			if (!entity) return;

			CScene::SerializeEntity(out, entity);			
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(pFilepath.c_str());
		fout << out.c_str();
	}

	void CScene::SerializeToBinary(const TString& pFilepath)
	{
		
	}

	bool CScene::DeserializeFromText(const TString& pFilepath)
	{
		YAML::Node data = YAML::LoadFile(pFilepath.c_str());
		if (!data["Scene"]) return false;

		TString SceneName = data["Scene"].as<TString>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				CEntity deserializedEntity = CreateEntity();

				MPL::forTypes<MyComponents>([&](auto t) {
					using tCompType = ECS_TYPE(t);

					if constexpr (tCompType::Flags & ECF::Serializable) {
						if (auto comp = entity[CComponentTraits::GetName<tCompType>()])
						{
							tCompType* component = nullptr;
							if constexpr (!(tCompType::Flags & ECF::AutoAddToAll))
								component = &deserializedEntity.AddComponent<tCompType>();
							else
								component = &deserializedEntity.GetComponent<tCompType>();
							
							component->Deserialize(comp);
						}
					}
					});

			}
		}
		return true;
	}

	bool CScene::DeserializeFromBinary(const TString& pFilepath)
	{
		return false;
	}
}