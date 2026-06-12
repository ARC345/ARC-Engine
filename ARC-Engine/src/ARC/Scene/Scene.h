#pragma once
#include "Component.h"
#include "ARC/Types/Vector.h"
#include "ARC/Types/Delegate.h"
#include "entt.hpp"
#include "ARC/Core/UUID.h"

class b2World;

namespace arc {

	using manager = entt::registry;

	class editor_camera;
	class entity;

	class scene
	{
	public:
		scene();
		~scene();
		
		static ref<scene> copy(const ref<scene> p);

		entity create_entity(const string& pName = "Entity", const uuid pUUID = uuid::generate());
		void remove_entity(entity Entity);
	
		inline size_t get_entity_count() const { return manager_.alive(); }
	
		inline manager& get_manager() { return manager_; }
		inline const manager& get_manager() const { return manager_; }
		
		void on_update_editor(float DeltaTime, editor_camera& pCamera);
		void on_update_runtime(float DeltaTime);
		void on_update_simulation(float pDeltaTime, editor_camera& pCamera);

		void on_viewport_resize(vec2<uint32_t> pNewSize);
		void on_set_scene_state(u8 pCurrentSceneState, u8 pNewSceneState); // Raw scene_state
		
		void serialize_to_text(const std::filesystem::path& pFilepath);
		void serialize_to_binary(const std::filesystem::path& pFilepath);
		bool deserialize_from_text(const std::filesystem::path& pFilepath);
		bool deserialize_from_binary(const std::filesystem::path& pFilepath);

		template<typename... Ts>
		decltype(auto) FilterByComponents() { return manager_.view<Ts...>(); }

		inline decltype(auto) get_viewport_size() { return viewport_size_; }

		entity get_primary_camera_entity();
		entity duplicate_entity(entity pSrcEntity);

	private:
		void begin_physics();
		void end_physics();
		
		void RenderScene();
		void update_physics(float pDeltaTime);

		void serialize_entity(YAML::Emitter& pOut, entity pEntity);
		void DeserializeEntity(YAML::Emitter& pOut, entity pEntity);
	public:
	private:
		vec2<u32> viewport_size_;
		
		manager manager_;
		b2World* physics_world_ = nullptr;

		delegate<bool(const string&)> tree_node_begin_func_;
		delegate<void()> tree_node_end_func_;
		
		friend class scene_hierarchy_panel;
	};
}