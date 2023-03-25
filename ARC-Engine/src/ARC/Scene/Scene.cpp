#include "arc_pch.h"
#include "Scene.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Core/Yaml.h"
#include "Entity.h"
#include "BasicComponents.h"
#include "SceneRegistry.h"
#include "ARC/Helpers/Math.h"
#include "ColliderComponents.h"
#include "PhysicsHandler.h"

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
		SSceneRegistry::SetupComponent<CAngularVelocityComponent>();
		SSceneRegistry::SetupComponent<CAccelerationComponent>();
		SSceneRegistry::SetupComponent<CAngularAccelerationComponent>();
		SSceneRegistry::SetupComponent<CCircleColliderComponent>();
		SSceneRegistry::SetupComponent<CPolyColliderComponent>();
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

	void CScene::OnUpdateRuntime(float pDeltaTime)
	{
		mManager.view<CNativeScriptComponent>().each([=](auto entity, auto& nativeScriptcomp)
			{
				if (!nativeScriptcomp.Controller)
				{
					nativeScriptcomp.Controller = nativeScriptcomp.InstantiateDelegate();
					nativeScriptcomp.Controller->Setup(nativeScriptcomp.OwningEntity);
					nativeScriptcomp.Controller->OnCreate();
				}

				nativeScriptcomp.Controller->OnUpdate(pDeltaTime);
			});

		HandleCircleXCircleCollision();
		HandleCircleXPolyCollision();
		HandlePolyXPolyCollision();

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

		//std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view](auto entity) {
		//	// ...
		//	}); 
		mManager.view<CTransform2DComponent, CVelocityComponent>().each([pDeltaTime](auto entity, auto& transformComp, auto& velocityComp)
			{
				SPhysyicsHandler::UpdateFrameLinearVelocity(transformComp.Transform, velocityComp.Velocity, pDeltaTime);
			});
		mManager.view<CTransform2DComponent, CAngularVelocityComponent>().each([pDeltaTime](auto entity, auto& transformComp, auto& angularVelocityComp)
			{
				SPhysyicsHandler::UpdateFrameAngularVelocity(transformComp.Transform, angularVelocityComp.AngularVelocity, pDeltaTime);
			});
		mManager.view<CVelocityComponent, CAccelerationComponent>().each([pDeltaTime](auto entity, auto& velocityComp, auto& accelerationComp)
			{
				SPhysyicsHandler::UpdateFrameAcceleration(velocityComp.Velocity, accelerationComp.Acceleration, pDeltaTime);
			});
		mManager.view<CAngularVelocityComponent, CAngularAccelerationComponent>().each([pDeltaTime](auto entity, auto& angularVelocityComp, auto& angularAccelerationComp)
			{
				SPhysyicsHandler::UpdateFrameAngularAcceleration(angularVelocityComp.AngularVelocity, angularAccelerationComp.AngularAcceleration, pDeltaTime);
			});
	}

	void CScene::HandleCircleXCircleCollision()
	{
		auto View = mManager.view<CCircleColliderComponent, CTransform2DComponent>();
		auto filterC = View.each();
		static FVec3 Mask = { 1, 1, 0 };

		if (View.size_hint() <= 1) return;
		
		for (auto i = filterC.begin(); i != filterC.end(); i++)
		{
			auto&& [e0_id, e0_ccc, e0_ctc] = *i;
			auto e0 = CEntity(e0_id, this);
			FVec3 CurrLoc = (e0_ctc.Transform.Location + e0_ccc.Location) * Mask;

			for (auto i2 = ++i; i2 != filterC.end(); i2++)
			{
				auto&& [e1_id, e1_ccc, e1_ctc] = *i2;
				auto e1 = CEntity(e1_id, this);

				if (e0_id == e1_id) continue;

				FVec3 CurrLoc1 = (e1_ctc.Transform.Location + e1_ccc.Location) * Mask;
				float depth;
				FVec3 normal;

				if (!SPhysyicsHandler::IsCollidingSphere(CurrLoc, CurrLoc1, e0_ccc.Radius + e1_ccc.Radius, normal, depth)) continue;
				SPhysyicsHandler::ClearOverlap(e0_ctc.Transform.Location, e1_ctc.Transform.Location, normal, depth);

				if (!(e0.HasComponent<CMassComponent>() && e1.HasComponent<CMassComponent>())) continue;
				if (!(e0.HasComponent<CVelocityComponent>() || e1.HasComponent<CVelocityComponent>())) continue;

				FVec3 ZeroVector = FVec3::ZeroVector();
				FVec3* CurrVel0;
				FVec3* CurrVel1;
				float CurrMass0;
				float CurrMass1;

				if (e0.HasComponent<CVelocityComponent>())
				{
					CurrVel0 = &e0.GetComponent<CVelocityComponent>().Velocity;
					CurrMass0 = e0.GetComponent<CMassComponent>().Mass;
				}
				else
				{
					CurrVel0 = &ZeroVector;
					CurrMass0 = std::numeric_limits<float>::infinity();
				}
				if (e1.HasComponent<CVelocityComponent>())
				{
					CurrVel1 = &e1.GetComponent<CVelocityComponent>().Velocity;
					CurrMass1 = e1.GetComponent<CMassComponent>().Mass;
				}
				else
				{
					CurrVel1 = &ZeroVector;
					CurrMass1 = std::numeric_limits<float>::infinity();
				}

				// resolve collision
				SPhysyicsHandler::HandleLinearCollisionResponse(*CurrVel0, *CurrVel1, CurrMass0, CurrMass1, normal, SMath::Clamp(SMath::Min(e0_ccc.Restitution, e1_ccc.Restitution), 0.f, 1.f));
			}
		}
	}

	void CScene::HandleCircleXPolyCollision()
	{
		static FVec3 Mask = { 1, 1, 0 };

		for (auto&& [e0_id, e0_cpc, e0_ctc] : mManager.view<CPolyColliderComponent, CTransform2DComponent>().each())
		{
			auto e0 = CEntity(e0_id, this);
			auto translatedVerts = e0_cpc.Vertices;

			auto cs0 = SMath::Cos(e0_ctc.Transform.Rotation);
			auto sn0 = SMath::Sin(e0_ctc.Transform.Rotation);

			std::for_each(translatedVerts.begin(), translatedVerts.end(), [&](auto& d) {
				d = {
					cs0 * d.x - sn0 * d.y + e0_ctc.Transform.Location.x,
					sn0 * d.x + cs0 * d.y + e0_ctc.Transform.Location.y,
					0
				};

				});
			for (auto&& [e1_id, e1_ccc, e1_ctc] : mManager.view<CCircleColliderComponent, CTransform2DComponent>().each())
			{
				auto e1 = CEntity(e1_id, this);
				FVec3 CurrLoc = (e1_ctc.Transform.Location + e1_ccc.Location) * Mask;
				
				FVec3 normal;
				float depth = 0.f;

				if (!SPhysyicsHandler::IsCollidingPolygon2DVsSphere(translatedVerts, CurrLoc, e1_ccc.Radius, normal, depth)) continue;
				SPhysyicsHandler::ClearOverlap(e0_ctc.Transform.Location, e1_ctc.Transform.Location, normal, depth);

				if (!(e0.HasComponent<CMassComponent>() && e1.HasComponent<CMassComponent>())) continue;
				if (!(e0.HasComponent<CVelocityComponent>() || e1.HasComponent<CVelocityComponent>())) continue;

				FVec3 ZeroVector = FVec3::ZeroVector();
				FVec3* CurrVel0;
				FVec3* CurrVel1;
				float CurrMass0;
				float CurrMass1;

				if (e0.HasComponent<CVelocityComponent>())
				{
					CurrVel0 = &e0.GetComponent<CVelocityComponent>().Velocity;
					CurrMass0 = e0.GetComponent<CMassComponent>().Mass;
				}
				else
				{
					CurrVel0 = &ZeroVector;
					CurrMass0 = std::numeric_limits<float>::infinity();
				}
				if (e1.HasComponent<CVelocityComponent>())
				{
					CurrVel1 = &e1.GetComponent<CVelocityComponent>().Velocity;
					CurrMass1 = e1.GetComponent<CMassComponent>().Mass;
				}
				else
				{
					CurrVel1 = &ZeroVector;
					CurrMass1 = std::numeric_limits<float>::infinity();
				}

				// resolve collision
				SPhysyicsHandler::HandleLinearCollisionResponse(
					*CurrVel0,
					*CurrVel1,
					CurrMass0,
					CurrMass1,
					normal,
					SMath::Clamp(SMath::Min(e0_cpc.Restitution, e1_ccc.Restitution),
						0.f,
						1.f
					));
			}
		}
	}

	void CScene::HandlePolyXPolyCollision()
	{
		auto View = mManager.view<CPolyColliderComponent, CTransform2DComponent>();
		auto filterC = View.each();
		static FVec3 Mask = { 1, 1, 0 };

		if (View.size_hint() <= 1) return;
		for (auto i = filterC.begin(); i != filterC.end(); i++)
		{
			auto&& [e0_id, e0_cpc, e0_ctc] = *i;
			auto e0 = CEntity(e0_id, this);
			auto translatedVerts = e0_cpc.Vertices;

			auto cs0 = SMath::Cos(e0_ctc.Transform.Rotation);
			auto sn0 = SMath::Sin(e0_ctc.Transform.Rotation);

			std::for_each(translatedVerts.begin(), translatedVerts.end(), [&](auto& d) {
				d = {
					cs0 * d.x - sn0 * d.y + e0_ctc.Transform.Location.x,
					sn0 * d.x + cs0 * d.y + e0_ctc.Transform.Location.y,
					0
				};

				});
			for (auto i2 = ++i; i2 != filterC.end(); i2++)
			{
				auto&& [e1_id, e1_cpc, e1_ctc] = *i2;
				auto e1 = CEntity(e1_id, this);
				auto translatedVerts1 = e1_cpc.Vertices;

				auto cs1 = SMath::Cos(e1_ctc.Transform.Rotation);
				auto sn1 = SMath::Sin(e1_ctc.Transform.Rotation);
				std::for_each(translatedVerts1.begin(), translatedVerts1.end(), [&](auto& d) {
					d = {
						cs1 * d.x - sn1 * d.y + e1_ctc.Transform.Location.x,
						sn1 * d.x + cs1 * d.y + e1_ctc.Transform.Location.y,
						0
					};
					});
				if (e0_id == e1_id) continue;
				
				FVec3 normal;
				float depth = 0.f;

				if (!SPhysyicsHandler::IsCollidingPolygon2D(translatedVerts, translatedVerts1, normal, depth)) continue;
				SPhysyicsHandler::ClearOverlap(e0_ctc.Transform.Location, e1_ctc.Transform.Location, normal, depth);

				if (!(e0.HasComponent<CMassComponent>() && e1.HasComponent<CMassComponent>())) continue;
				if (!(e0.HasComponent<CVelocityComponent>() || e1.HasComponent<CVelocityComponent>())) continue;

				FVec3 ZeroVector = FVec3::ZeroVector();
				FVec3* CurrVel0;
				FVec3* CurrVel1;
				float CurrMass0;
				float CurrMass1;

				if (e0.HasComponent<CVelocityComponent>())
				{
					CurrVel0 = &e0.GetComponent<CVelocityComponent>().Velocity;
					CurrMass0 = e0.GetComponent<CMassComponent>().Mass;
				}
				else
				{
					CurrVel0 = &ZeroVector;
					CurrMass0 = std::numeric_limits<float>::infinity();
				}
				if (e1.HasComponent<CVelocityComponent>())
				{
					CurrVel1 = &e1.GetComponent<CVelocityComponent>().Velocity;
					CurrMass1 = e1.GetComponent<CMassComponent>().Mass;
				}
				else
				{
					CurrVel1 = &ZeroVector;
					CurrMass1 = std::numeric_limits<float>::infinity();
				}

				// resolve collision
				SPhysyicsHandler::HandleLinearCollisionResponse(
					*CurrVel0,
					*CurrVel1,
					CurrMass0,
					CurrMass1,
					normal,
					SMath::Clamp(SMath::Min(e0_cpc.Restitution, e1_cpc.Restitution),
						0.f,
						1.f
					));
			}
		}
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

	bool CScene::DeserializeFromBinary(const std::filesystem::path& pFilepath)
	{
		return false;
	}
}