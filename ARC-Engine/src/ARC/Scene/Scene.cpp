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
#include "box2d/b2_circle_shape.h"

namespace arc {
	scene::scene()
	{
		scene_registry::setup_component<id_component>();
		scene_registry::setup_component<name_component>();
		scene_registry::setup_component<transform_2d_component>();
		scene_registry::setup_component<sprite_renderer_component>();
		scene_registry::setup_component<camera_component>();
		scene_registry::setup_component<native_script_component>();
		scene_registry::setup_component<rigid_body_2d_component>();
		scene_registry::setup_component<box_collider_2d_component>();
		scene_registry::setup_component<circle_collider_2d_component>();
		scene_registry::setup_component<circle_renderer_component>();
	}

	scene::~scene()
	{
		if (physics_world_) end_physics(); // just in case
	}

	ref<scene> scene::copy(const ref<scene> p)
	{
		ref<scene> cpy = create_ref<scene>();
		cpy->viewport_size_ = p->viewport_size_;

		p->manager_.view<id_component>().each([=](auto iID, auto iUUID){
			cpy->duplicate_entity(entity(iID, p.get()));
		});
		
		return cpy;
	}

	entity scene::create_entity(const string& pName, const uuid pUUID)
	{
		entity entity(manager_.create(), this);

		entity.add_component<id_component>(pUUID);
		entity.add_component<name_component>(pName);

		return entity;
	}

	void scene::remove_entity(entity Entity)
	{
		manager_.destroy(Entity.get_id());
		Entity.on_kill();
	}

	void scene::on_update_editor(float DeltaTime, editor_camera& pCamera)
	{
		renderer_2d::begin_scene(pCamera);
		RenderScene();
		renderer_2d::end_scene();
	}

	void scene::on_update_runtime(float pDeltaTime)
	{
		manager_.view<native_script_component>().each([pDeltaTime](auto entity, auto& nativeScriptcomp)
			{
				if (!nativeScriptcomp.Controller)
				{
					nativeScriptcomp.Controller = nativeScriptcomp.InstantiateDelegate();
					nativeScriptcomp.Controller->set_up(nativeScriptcomp.OwningEntity);
					nativeScriptcomp.Controller->on_create();
				}

				nativeScriptcomp.Controller->on_update(pDeltaTime);
			});

		update_physics(pDeltaTime);

		if (auto primaryCameraEntity = get_primary_camera_entity())
		{
			renderer_2d::begin_scene(primaryCameraEntity.get_component<camera_component>().Camera, primaryCameraEntity.get_component<transform_2d_component>().Transform);
			RenderScene();
			renderer_2d::end_scene();
		}

		//std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view](auto entity) {
		//	// ...
		//	}); 
	}

	void scene::on_update_simulation(float pDeltaTime, editor_camera& pCamera)
	{
		renderer_2d::begin_scene(pCamera);
		RenderScene();
		update_physics(pDeltaTime);
		renderer_2d::end_scene();
	}

	void scene::on_viewport_resize(vec2<uint32_t> pNewSize)
	{
		viewport_size_ = pNewSize;

		manager_.view<camera_component>().each([=](auto entity, auto& cameraComp)
			{
				//if (cameraComp.bFixedAspectRatio) return @todo
				cameraComp.Camera.set_viewport_size(pNewSize);
			});
	}

	void scene::begin_physics(){
		on_viewport_resize(viewport_size_);
		physics_world_ = new b2World({ 0.0f, -9.8f });
		manager_.view<transform_2d_component, rigid_body_2d_component>().each([&](auto e, auto& transformComponent, auto& rigidBody2dComponent) {
			entity entity = { e, this };
			b2BodyDef bodyDef;
			bodyDef.type = (b2BodyType)rigidBody2dComponent.Type;
			bodyDef.position.Set(transformComponent.Transform.Location.x, transformComponent.Transform.Location.y);
			bodyDef.angle = transformComponent.Transform.Rotation;
			bodyDef.fixedRotation = rigidBody2dComponent.bFixedRotation;
			auto* body = physics_world_->CreateBody(&bodyDef);
			rigidBody2dComponent.RuntimeBody = body;
			if (entity.has_component<box_collider_2d_component>())
			{
				auto& colliderComponent = entity.get_component<box_collider_2d_component>();
				b2PolygonShape boxShape;
				boxShape.set_as_box(
					transformComponent.Transform.Scale.x * colliderComponent.size.x,
					transformComponent.Transform.Scale.y * colliderComponent.size.y
				);

				b2FixtureDef fixureDef;
				fixureDef.shape = &boxShape;
				fixureDef.density = colliderComponent.Density;
				fixureDef.friction = colliderComponent.Friction;
				fixureDef.restitution = colliderComponent.Restitution;
				fixureDef.restitutionThreshold = colliderComponent.RestitutionThreshhold;

				colliderComponent.RuntimeFixture = body->CreateFixture(&fixureDef);
			}
			if (entity.has_component<circle_collider_2d_component>())
			{
				auto& colliderComponent = entity.get_component<circle_collider_2d_component>();
				b2CircleShape circleShape;
				circleShape.m_p.Set(colliderComponent.Offset.x, colliderComponent.Offset.y);
				circleShape.m_radius = colliderComponent.Radius * transformComponent.Transform.Scale.x;

				b2FixtureDef fixureDef;
				fixureDef.shape = &circleShape;
				fixureDef.density = colliderComponent.Density;
				fixureDef.friction = colliderComponent.Friction;
				fixureDef.restitution = colliderComponent.Restitution;
				fixureDef.restitutionThreshold = colliderComponent.RestitutionThreshhold;

				colliderComponent.RuntimeFixture = body->CreateFixture(&fixureDef);
			}
		});
	}

	void scene::end_physics()
	{
		delete physics_world_;
		physics_world_ = nullptr;
	}

	void scene::RenderScene()
	{
		manager_.group<transform_2d_component>(entt::get<sprite_renderer_component>).each([](auto entity, auto& transformComp, auto& spriteRendererComp)
			{
				static primitive_2d Quad;
				Quad.Color = spriteRendererComp.Color;
				Quad.Texture = spriteRendererComp.Texture;
				Quad.TextureScaling = spriteRendererComp.TextureScaling;
				Quad.Transform = transformComp;
				Quad.TransparencyLevel = (Quad.Texture || !math::is_equal(Quad.Color.a, 1.f, 0.01f)) ? transparency_type::Translucent : transparency_type::Opaque;
				renderer_2d::draw_quad(Quad, int(entity));
			});
		manager_.view<transform_2d_component, circle_renderer_component>().each([](auto entity, auto& transformComp, auto& circleRendererComp)
			{
				renderer_2d::draw_circle(
					transformComp.Transform.Location,
					transformComp.Transform.Rotation,
					transformComp.Transform.Scale,
					circleRendererComp.Color,
					circleRendererComp.Thickness,
					circleRendererComp.Sharpness,
					int(entity));
			});
	}

	void scene::update_physics(float pDeltaTime)
	{
		physics_world_->Step(pDeltaTime, 6, 2); /// expose to editor
		manager_.view<transform_2d_component, rigid_body_2d_component>().each([&](auto e, auto& transformComponent, auto& rb2dComponent) {
				entity entity = {e, this};
				b2Body* body = (b2Body*)rb2dComponent.RuntimeBody;
				transformComponent.Transform.Location.x = body->get_position().x;
				transformComponent.Transform.Location.y = body->get_position().y;
				transformComponent.Transform.Rotation = body->get_angle();
			});
	}

	/*
	* None = 0,
	* Edit=1,
	* Play=2,
	* Simulate=3
	*/
	void scene::on_set_scene_state(u8 pCurrentSceneState, u8 pNewSceneState)
	{
		switch (pCurrentSceneState)
		{
		case 0:
			break;
		case 1:
			//end Edit
			break;
		case 2:
			//end Play
			end_physics();
			break;
		case 3:
			//end Simulate
			end_physics();
		default:
			ARC_CORE_ASSERT("Invalid (Current) Scene State");
		}
		switch (pNewSceneState)
		{
		case 0:
			break;
		case 1:
			//Start Edit
			break;
		case 2:
			//Start Play
			begin_physics();

			break;
		case 3:
			//Start Simulate
			begin_physics();
			
			break;
		default:
			ARC_CORE_ASSERT("Invalid (New) Scene State");
		}
	}

	void scene::serialize_entity(YAML::Emitter& pOut, entity pEntity)
	{
		pOut << YAML::BeginMap;
		pOut << YAML::Key << "Entity" << YAML::Value << pEntity.get_component<id_component>().ID; // @TODO
		pOut << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;

		for (auto& compMeta : scene_registry::get_meta_components())
			if (auto* compPtr = compMeta.second.get_component(pEntity))
				if (compPtr->get_flags() & ecf::component_flags::Serializable)
				{
					pOut << YAML::BeginMap;
					pOut << YAML::Key << "Name" << YAML::Value << compMeta.second.get_name();
					pOut << YAML::Key << "Data" << YAML::Value << YAML::BeginMap;
					compPtr->serialize(pOut);
					pOut << YAML::EndMap;
					pOut << YAML::EndMap;
				}

		pOut << YAML::EndSeq;
		pOut << YAML::EndMap;
	}

	void scene::serialize_to_text(const std::filesystem::path& pFilepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		manager_.each([&](const entity_id pEID)
			{
				entity entity = { pEID, this };
				if (!entity) return;

				scene::serialize_entity(out, entity);			
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(pFilepath.c_str());
		fout << out.c_str();
	}

	void scene::serialize_to_binary(const std::filesystem::path& pFilepath)
	{
		
	}

	bool scene::deserialize_from_text(const std::filesystem::path& pFilepath)
	{
		YAML::Node data = YAML::LoadFile(pFilepath.string());
		if (!data["Scene"]) return false;

		string SceneName = data["Scene"].as<string>();

		auto entities = data["Entities"];
		for (auto entity : entities)
		{
			entity deserializedEntity = create_entity("Entity", entity["Entity"].as<uuid>());
			
			for (auto compData : entity["Components"])
			{
				auto Node = compData["Data"];

				if (compData["Name"].as<string>() == component_traits::get_name<name_component>()) {
					deserializedEntity.get_component<name_component>().deserialize(Node);
					continue;
				}
				
				auto compName = compData["Name"].as<string>();

				scene_registry::get_meta_components()[scene_registry::get_registered_component_name_id_map()[compData["Name"].as<string>()]].add_component(deserializedEntity)->deserialize(Node);
			}
		}
		return true;
	}

	bool scene::deserialize_from_binary(const std::filesystem::path& pFilepath)
	{
		return false;
	}

	entity scene::get_primary_camera_entity()
	{
		entity rval;
		manager_.view<transform_2d_component, camera_component>().each([&](auto entity, auto& transformComp, auto& cameraComp)
			{
				if (cameraComp.Primary)
				{
					rval = { entity, this };
					return;
				}
			});
		return rval;
	}

	entity scene::duplicate_entity(entity pSrcEntity)
	{
		auto dstEntity = create_entity(pSrcEntity.get_name(), pSrcEntity.get_uuid());
		for (auto i : scene_registry::get_meta_components()) {
			if (i.first == component_traits::get_id<name_component>() || i.first == component_traits::get_id<id_component>()) continue;
			i.second.copy_component(dstEntity, pSrcEntity);
		}
		return dstEntity;
	}

}