#include "arc_pch.h"
#include "Scene.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Core/Yaml.h"
#include "Entity.h"
#include "BasicComponents.h"
#include "SceneRegistry.h"
#include "../Helpers/Math.h"
#include "ColliderComponents.h"

namespace ARC {
	CScene::CScene()
	{
		SSceneRegistry::SetupComponent<CNameComponent>();
		SSceneRegistry::SetupComponent<CTransform2DComponent>();
		SSceneRegistry::SetupComponent<CSpriteRendererComponent>();
		SSceneRegistry::SetupComponent<CCameraComponent>();
		SSceneRegistry::SetupComponent<CNativeScriptComponent>();
		SSceneRegistry::SetupComponent<CMassComponent>();
		SSceneRegistry::SetupComponent<CVelocityComponent>();
		SSceneRegistry::SetupComponent<CCircleColliderComponent>();
	}

	CScene::~CScene()
	{

	}

	CEntity CScene::CreateEntity(const TString& pName)
	{
		CEntity entity(mManager.create(), this);

		entity.AddComponent<CNameComponent>(pName);

		return entity;
	}

	void CScene::RemoveEntity(CEntity Entity)
	{
		mManager.destroy(Entity.GetID());
		Entity.OnKill();
	}

	void CScene::OnUpdateEditor(float DeltaTime, CEditorCamera& pCamera)
	{
		CRenderer2D::BeginScene(pCamera);
		mManager.group<CTransform2DComponent>(entt::get<CSpriteRendererComponent>).each([](auto entity, auto& transformComp, auto& spriteRendererComp)
			{
				static CPrimitive2D Quad;
				Quad.Color = spriteRendererComp.Color;
				Quad.Texture = spriteRendererComp.Texture;
				Quad.TextureScaling = spriteRendererComp.TextureScaling;
				Quad.Transform = transformComp;
				Quad.TransparencyLevel = (Quad.Texture && !SMath::IsEqual(Quad.Color.a, 1.f, 0.01f)) ? ETransparencyType::Translucent : ETransparencyType::Opaque;
				CRenderer2D::DrawQuad(Quad, int(entity));
			});

		CRenderer2D::EndScene();
	}

	void CScene::OnUpdateRuntime(float DeltaTime)
	{
		mManager.view<CNativeScriptComponent>().each([=](auto entity, auto& nativeScriptcomp)
			{
				if (!nativeScriptcomp.Controller)
				{
					nativeScriptcomp.Controller = nativeScriptcomp.InstantiateDelegate();
					nativeScriptcomp.Controller->Setup(nativeScriptcomp.OwningEntity);
					nativeScriptcomp.Controller->OnCreate();
				}

				nativeScriptcomp.Controller->OnUpdate(DeltaTime);
			});
		
		CCamera* mainCam = nullptr;
		FTransform2D* camTransform = nullptr;
		
		mManager.view<CTransform2DComponent, CCameraComponent>().each([&](auto entity, auto& transformComp, auto& cameraComp)
			{
				if (cameraComp.bPrimary)
				{
					mainCam = &cameraComp.Camera;
					camTransform = &transformComp.Transform;
					return;
				}
			});

		if (mainCam)
		{
			CRenderer2D::BeginScene(*mainCam, *camTransform);
			mManager.group<CTransform2DComponent>(entt::get<CSpriteRendererComponent>).each([](auto entity, auto& transformComp, auto& spriteRendererComp)
				{
					static CPrimitive2D Quad;
					Quad.Color = spriteRendererComp.Color;
					Quad.Texture = spriteRendererComp.Texture;
					Quad.TextureScaling = spriteRendererComp.TextureScaling;
					Quad.Transform = transformComp;
					Quad.TransparencyLevel = (Quad.Texture && !SMath::IsEqual(Quad.Color.a, 1.f, 0.01f)) ? ETransparencyType::Translucent : ETransparencyType::Opaque;
					CRenderer2D::DrawQuad(Quad, int(entity));
				});

			CRenderer2D::EndScene();
		}

		mManager.view<CTransform2DComponent, CVelocityComponent>().each([](auto entity, auto& transformComp, auto& velocityComp)
			{
				transformComp.Transform.Location += velocityComp.Velocity;
			});
	}

	void CScene::OnViewportResize(TVec2<uint32_t> pNewSize)
	{
		mViewportSize = pNewSize;

		mManager.view<CCameraComponent>().each([=](auto entity, auto& cameraComp)
			{
				if (cameraComp.bFixedAspectRatio) return
				cameraComp.Camera.SetViewportSize(pNewSize);
			});
	}

	void CScene::SerializeEntity(YAML::Emitter& pOut, CEntity pEntity)
	{
		pOut << YAML::BeginMap;
		pOut << YAML::Key << "Entity" << YAML::Value << "1038845309209"; // @TODO
		pOut << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;

		for (auto& comp : SSceneRegistry::GetTypeErasedGetComponentFuncs())
			if (auto* compPtr = comp.second(pEntity))
				if (compPtr->GetFlags() & ECF::EComponentFlags::Serializable)
				{
					pOut << YAML::BeginMap;
					pOut << YAML::Key << "Name" << YAML::Value << comp.first;
					pOut << YAML::Key << "Data" << YAML::Value << YAML::BeginMap;
					compPtr->Serialize(pOut);
					pOut << YAML::EndMap;
					pOut << YAML::EndMap;
				}

		pOut << YAML::EndSeq;
		pOut << YAML::EndMap;
	}

	void CScene::SerializeToText(const TString& pFilepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		mManager.each([&](const TEntityID pEID)
			{
				CEntity entity = { pEID, this };
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
		for (auto entity : entities)
		{
			uint64_t uuid = entity["Entity"].as<uint64_t>();
			CEntity deserializedEntity = CreateEntity();

			for (auto compData : entity["Components"])
			{
				auto Node = compData["Data"];

				if (compData["Name"].as<TString>() == CComponentTraits::GetName<CNameComponent>()) {
					deserializedEntity.GetComponent<CNameComponent>().Deserialize(Node);
					continue;
				}

				SSceneRegistry::GetTypeErasedAddComponentFuncs()[compData["Name"].as<TString>()](deserializedEntity)->Deserialize(Node);
			}
		}
		return true;
	}

	bool CScene::DeserializeFromBinary(const TString& pFilepath)
	{
		return false;
	}
}