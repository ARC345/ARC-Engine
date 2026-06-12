#pragma once
#include "Component.h"
#include "BasicComponents.h"
#include "entt.hpp"
#include "Scene.h"
#include "ARC/Core/UUID.h"

namespace arc { class scene; }
namespace arc { class entity; }

namespace arc {
	using entity_id = entt::entity;
	
	class entity
	{
	public:
		entity() =default;
		entity(entity_id pHandle, scene* pScene) : entity_(pHandle), scene_(pScene) {};
		entity(const entity&) = default;

		entity_id get_id() const { return entity_; }
		const string& get_name() { return get_component<name_component>().Name; }
		uuid get_uuid() { return get_component<id_component>().ID; }

		bool is_valid() const
		{
			return entity_ != entt::null && scene_ != nullptr;
		}
		template<typename T>
		void remove_component() {
			ARC_CORE_ASSERT(scene_ && has_component<T>())
			scene_->get_manager().remove<T>(entity_);
		}
		template<typename T>
		[[nodiscard]] inline bool has_component() const { 
			ARC_CORE_ASSERT(scene_);
			return scene_ && scene_->get_manager().all_of<T>(entity_);
		}
		template<typename T>
		[[nodiscard]] inline T& get_component() {
			ARC_CORE_ASSERT(scene_ && has_component<T>());
			auto& rval = scene_->get_manager().get<T>(entity_);
			return rval;
		}
		template<typename T>
		[[nodiscard]] inline T& get_component() const {
			ARC_CORE_ASSERT(scene_ && has_component<T>());
			auto& rval = scene_->get_manager().get<T>(entity_);
			return rval;
		}
		template<typename T, typename ... Ts>
		inline T& add_component(Ts&&... pArgs) {
			ARC_CORE_ASSERT(scene_)
			auto& comp = scene_->get_manager().emplace<T>(entity_, std::forward<Ts>(pArgs)...);
			comp.on_construct(*this);

			return comp;
		}
		
		void Kill()
		{
			scene_->get_manager().destroy(entity_);
			on_kill();
		}

		entity Duplicate()
		{
			return scene_->duplicate_entity(*this);
		}

		operator bool() const { return entity_ != entt::null && scene_->get_manager().valid(entity_); }
		bool operator == (const entity _) const { return entity_ == _.entity_ && scene_ == _.scene_; }
		bool operator != (const entity _) const { return !(*this == _); }
	protected:
		virtual void on_kill()
		{
			entity_ = entt::null;
			scene_ = nullptr;
		}
	private:
		entity_id entity_ = entt::null;
		scene* scene_ = nullptr;


		friend class scene;
	};
}