#pragma once

#include "ARC/Scene/Scene.h"
#include "ARC/Scene/Entity.h"

namespace arc {

	struct component_traits
	{
		template<typename T>
		static constexpr inline
		bool is_component() { return std::is_base_of_v<component_base, T>; }

		template<typename T>
		static constexpr inline
		decltype(auto) get_name()
		{
			static_assert(is_component<T>());
			return shpr::get_class_name<T>();
		}

		template<typename T>
		static inline
		auto& get_flags()
		{
			static_assert(is_component<T>());
			return T::Flags;
		}

		template<typename T>
		static constexpr const inline
		auto get_id() noexcept {
			static const auto value = ItrlCounter();
			return value;
		}

	private:
		static inline
			auto ItrlCounter() noexcept {
			static u32 value = 0;
			return value++;
		}
	};

	struct scene_registry
	{
		struct meta_component {
			delegate<void(entity pEntity)> remove_component;
			delegate<component_base* (entity pEntity)> get_component;
			delegate<component_base* (entity pEntity)> add_component;
			delegate<component_base* (entity pDstEntity, const entity pSrcEntity)> copy_component;
			delegate<u32& ()> get_flags;
			delegate<string ()> get_name;
		};

		template<typename T>
		static void setup_component()
		{
			static bool bRet = false;
			if(bRet) return;
			bRet = true;

			const string& compName = component_traits::get_name<T>();
			const auto compID = component_traits::get_id<T>();
			registered_component_names_.push_back(compName);
			registered_component_name_id_map_[compName] = compID;
			
			auto& mc = meta_components_.emplace(compID, meta_component()).first->second;
			mc.get_component = [](entity pEntity) -> component_base* {
				static_assert(component_traits::is_component<T>());
				if (pEntity && pEntity.has_component<T>())
					return &pEntity.get_component<T>();
				return nullptr;
					};
			mc.add_component = [](entity pEntity) -> component_base* {
				static_assert(component_traits::is_component<T>());
				if (pEntity && !pEntity.has_component<T>())
					return &pEntity.add_component<T>();
				return nullptr;
				};
			mc.remove_component = [](entity pEntity) {
				static_assert(component_traits::is_component<T>());
				if (pEntity && pEntity.has_component<T>())
					pEntity.remove_component<T>();
				};
			mc.copy_component = [](entity pDstEntity, const entity pSrcEntity) -> component_base* {
				static_assert(component_traits::is_component<T>());
				if (pDstEntity && pSrcEntity && pSrcEntity.has_component<T>())
					return &pDstEntity.add_component<T>(pSrcEntity.get_component<T>());
				return nullptr;
				};
			mc.get_flags = []() -> u32& {
				static_assert(component_traits::is_component<T>());
				return component_traits::get_flags<T>();
				};
			mc.get_name = []() {
				static_assert(component_traits::is_component<T>());
				return component_traits::get_name<T>();
				};
		};

		static auto& get_registered_components_names() { return registered_component_names_; }
		static auto& get_registered_component_name_id_map() { return registered_component_name_id_map_; }
		static auto& get_meta_components() { return meta_components_; }

	private:
		// @Editor functionality @ TODO wrap around macro
	
		static inline std::vector<string> registered_component_names_;
		static inline std::unordered_map<string, u32> registered_component_name_id_map_;
		static inline std::unordered_map<u32, meta_component> meta_components_;

	};

}