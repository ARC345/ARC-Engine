#include "arc_pch.h"
#include "Scene.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Core/Yaml.h"
#include "Entity.h"
#include "SceneRegistry.h"
#include "ARC/Helpers/Math.h"
#include "PhysicsHandler.h"
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace ARC {
	CScene::CScene()
	{
		SSceneRegistry::SetupComponent<CIDComponent>();
		SSceneRegistry::SetupComponent<CNameComponent>();
		SSceneRegistry::SetupComponent<CTransform2DComponent>();
		SSceneRegistry::SetupComponent<CSpriteRendererComponent>();
		SSceneRegistry::SetupComponent<CCameraComponent>();
		SSceneRegistry::SetupComponent<CNativeScriptComponent>();
		SSceneRegistry::SetupComponent<CMassComponent>();
		SSceneRegistry::SetupComponent<CRigidBody2DComponent>();
		SSceneRegistry::SetupComponent<CBoxCollider2DComponent>();
		SSceneRegistry::SetupComponent<CCircleRendererComponent>();
	}

	CScene::~CScene()
	{

	}

	TRef<CScene> CScene::Copy(const TRef<CScene> p)
	{
		TRef<CScene> cpy = CreateRef<CScene>();
		cpy->mViewportSize = p->mViewportSize;

		p->mManager.view<CIDComponent>().each([=](auto iID, auto iUUID){
			cpy->DuplicateEntity(CEntity(iID, p.get()));
		});
		
		return cpy;
	}

	CEntity CScene::CreateEntity(const TString& pName, const TUUID pUUID)
	{
		CEntity entity(mManager.create(), this);

		entity.AddComponent<CIDComponent>(pUUID);
		entity.AddComponent<CNameComponent>(pName);

		return entity;
	}

	void CScene::RemoveEntity(CEntity Entity)
	{
		mManager.destroy(Entity.GetID());
		Entity.OnKill();
	}

	void CScene::OnRuntimeBegin()
	{
		if(dOnRuntimeBegin) dOnRuntimeBegin();
		mWorld = new b2World({0.0f, -9.8f});
		mManager.view<CTransform2DComponent, CRigidBody2DComponent>().each([&](auto e, auto& transformComponent, auto& rigidBody2dComponent) {
				CEntity entity = {e, this};
				b2BodyDef bodyDef;
				bodyDef.type = (b2BodyType)rigidBody2dComponent.Type;
				bodyDef.position.Set(transformComponent.Transform.Location.x, transformComponent.Transform.Location.y);
				bodyDef.angle = transformComponent.Transform.Rotation;
				bodyDef.fixedRotation = rigidBody2dComponent.bFixedRotation;
				auto* body = mWorld->CreateBody(&bodyDef);
				rigidBody2dComponent.RuntimeBody = body;
				if (entity.HasComponent<CBoxCollider2DComponent>())
				{
					auto& colliderComponent = entity.GetComponent<CBoxCollider2DComponent>();
					b2PolygonShape boxShape;
					boxShape.SetAsBox(
						transformComponent.Transform.Scale.x * colliderComponent.Size.x,
						transformComponent.Transform.Scale.y * colliderComponent.Size.y
						);

					b2FixtureDef fixureDef;
					fixureDef.shape = &boxShape;
					fixureDef.density = colliderComponent.Density;
					fixureDef.friction = colliderComponent.Friction;
					fixureDef.restitution = colliderComponent.Restitution;
					fixureDef.restitutionThreshold = colliderComponent.RestitutionThreshhold;

					colliderComponent.RuntimeFixture = body->CreateFixture(&fixureDef);
				}
			});
	}

	void CScene::OnRuntimeEnd()
	{
		if(dOnRuntimeEnd) dOnRuntimeEnd();
		delete mWorld;
		mWorld = nullptr;
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
		mManager.view<CTransform2DComponent, CCircleRendererComponent>().each([](auto entity, auto& transformComp, auto& circleRendererComp)
			{
				CRenderer2D::DrawCircle(
					transformComp.Transform.Location,
					transformComp.Transform.Rotation,
					transformComp.Transform.Scale,
					circleRendererComp.Color,
					circleRendererComp.Thickness,
					circleRendererComp.Sharpness,
					int(entity));
			});

		CRenderer2D::EndScene();
	}

	void CScene::OnUpdateRuntime(float pDeltaTime)
	{
		mManager.view<CNativeScriptComponent>().each([pDeltaTime](auto entity, auto& nativeScriptcomp)
			{
				if (!nativeScriptcomp.Controller)
				{
					nativeScriptcomp.Controller = nativeScriptcomp.InstantiateDelegate();
					nativeScriptcomp.Controller->Setup(nativeScriptcomp.OwningEntity);
					nativeScriptcomp.Controller->OnCreate();
				}

				nativeScriptcomp.Controller->OnUpdate(pDeltaTime);
			});

		{
			mWorld->Step(pDeltaTime, 6, 2); /// expose to editor
			mManager.view<CTransform2DComponent, CRigidBody2DComponent>().each([&](auto e, auto& transformComponent, auto& rb2dComponent) {
					CEntity entity = {e, this};
					b2Body* body = (b2Body*)rb2dComponent.RuntimeBody;
					transformComponent.Transform.Location.x = body->GetPosition().x;
					transformComponent.Transform.Location.y = body->GetPosition().y;
					transformComponent.Transform.Rotation = body->GetAngle();
				});
		}
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
			mManager.view<CTransform2DComponent, CCircleRendererComponent>().each([](auto entity, auto& transformComp, auto& circleRendererComp)
				{
					CRenderer2D::DrawCircle(
						transformComp.Transform.Location,
						transformComp.Transform.Rotation,
						transformComp.Transform.Scale,
						circleRendererComp.Color,
						circleRendererComp.Thickness,
						circleRendererComp.Sharpness,
						int(entity));
				});
			CRenderer2D::EndScene();
		}

		//std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view](auto entity) {
		//	// ...
		//	}); 
	}

	void CScene::OnViewportResize(TVec2<uint32_t> pNewSize)
	{
		mViewportSize = pNewSize;

		mManager.view<CCameraComponent>().each([=](auto entity, auto& cameraComp)
			{
				//if (cameraComp.bFixedAspectRatio) return @todo
				cameraComp.Camera.SetViewportSize(pNewSize);
			});
	}

	void CScene::SerializeEntity(YAML::Emitter& pOut, CEntity pEntity)
	{
		pOut << YAML::BeginMap;
		pOut << YAML::Key << "Entity" << YAML::Value << pEntity.GetComponent<CIDComponent>().ID; // @TODO
		pOut << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;

		for (auto& compMeta : SSceneRegistry::GetMetaComponents())
			if (auto* compPtr = compMeta.second.GetComponent(pEntity))
				if (compPtr->GetFlags() & ECF::EComponentFlags::Serializable)
				{
					pOut << YAML::BeginMap;
					pOut << YAML::Key << "Name" << YAML::Value << compMeta.second.GetName();
					pOut << YAML::Key << "Data" << YAML::Value << YAML::BeginMap;
					compPtr->Serialize(pOut);
					pOut << YAML::EndMap;
					pOut << YAML::EndMap;
				}

		pOut << YAML::EndSeq;
		pOut << YAML::EndMap;
	}

	void CScene::SerializeToText(const std::filesystem::path& pFilepath)
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

	void CScene::SerializeToBinary(const std::filesystem::path& pFilepath)
	{
		
	}

	bool CScene::DeserializeFromText(const std::filesystem::path& pFilepath)
	{
		YAML::Node data = YAML::LoadFile(pFilepath.string());
		if (!data["Scene"]) return false;

		TString SceneName = data["Scene"].as<TString>();

		auto entities = data["Entities"];
		for (auto entity : entities)
		{
			CEntity deserializedEntity = CreateEntity("Entity", entity["Entity"].as<TUUID>());
			
			for (auto compData : entity["Components"])
			{
				auto Node = compData["Data"];

				if (compData["Name"].as<TString>() == SComponentTraits::GetName<CNameComponent>()) {
					deserializedEntity.GetComponent<CNameComponent>().Deserialize(Node);
					continue;
				}

				SSceneRegistry::GetMetaComponents()[SSceneRegistry::GetRegisteredComponentNameIDMap()[compData["Name"].as<TString>()]].GetComponent(deserializedEntity)->Deserialize(Node);
			}
		}
		return true;
	}

	bool CScene::DeserializeFromBinary(const std::filesystem::path& pFilepath)
	{
		return false;
	}

	CEntity CScene::DuplicateEntity(CEntity pSrcEntity)
	{
		auto dstEntity = CreateEntity(pSrcEntity.GetName(), pSrcEntity.GetUUID());
		for (auto i : SSceneRegistry::GetMetaComponents()) {
			if (i.first == SComponentTraits::GetID<CNameComponent>() || i.first == SComponentTraits::GetID<CIDComponent>()) continue;
			i.second.CopyComponent(dstEntity, pSrcEntity);
		}
		return dstEntity;
	}

}