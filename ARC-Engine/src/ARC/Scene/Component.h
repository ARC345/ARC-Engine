#pragma once

#include "ARC/Types/Transform2D.h"
#include "ARC/Types/Delegate.h"
#include "ARC/Types/Color.h"
#include "ARC/Renderer/Texture.h"
#include "SceneCamera.h"
#include "Controller.h"
#include "entt.hpp"
#include <iostream>

namespace YAML { class Emitter; }
namespace YAML { class Node; }

namespace ARC {
	class CEntity;

	namespace ECF {
		enum EComponentFlags : uint32_t
		{
			ComponentFlagsNone = 0,
			ShowInPropertiesPanel = 1 << 0,
			ShowHeader = 1 << 1,
			Serializable = 1 << 2,
			DefaultComponentFlags = ShowInPropertiesPanel | ShowHeader
		};
	}

	struct CComponentBase
	{
		virtual void OnConstruct(CEntity& pOwningEntity){};
		virtual void DrawPropertiesUI(CEntity& pEntity) {};
		virtual void Serialize(YAML::Emitter& pOut) {};
		virtual void Deserialize(YAML::Node& pData) {};
		virtual uint32_t GetFlags()=0;

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags;
		static constexpr int32_t OnTopPriority = 0; // >1 top priority, <1 means bottom priority in scene heirachy panel @TODO implemeant
	};

	struct CComponentTraits
	{
		template<typename T>
		static constexpr bool IsComponent()
		{
			return std::is_base_of_v<CComponentBase, T>;
		}
		template<typename T>
		static constexpr decltype(auto) GetName()
		{
			static_assert(IsComponent<T>());
			return SHPR::GetClassName<T>();
		}
		template<typename T>
		static constexpr decltype(auto) GetFlags()
		{
			static_assert(IsComponent<T>());
			return T::Flags;
		}
		template<typename T>
		static constexpr decltype(auto) GetID() noexcept {
			static const std::size_t value = ItrlCounter();
			return value;
		}

	private:
		static std::size_t ItrlCounter() noexcept {
			static std::size_t value = 0;
			return value++;
		}

	};
}