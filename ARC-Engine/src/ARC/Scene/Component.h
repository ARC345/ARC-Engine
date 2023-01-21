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

	namespace ITRL::META {
		template<typename T>
		static void GetFlags(ECF::EComponentFlags& pOut)
		{
			pOut = (ECF::EComponentFlags)T::Flags;
		}
	}

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
			return HPR::GetClassName<T>();
		}
		template<typename T>
		static constexpr decltype(auto) GetNameHash()
		{
			return GetNameHash(GetName<T>());
		}
		static constexpr decltype(auto) GetNameHash(const TString& pName)
		{
			return entt::hashed_string::value(pName.c_str(), pName.size());
		}
		template<typename T>
		static constexpr decltype(auto) GetFlags()
		{
			return T::Flags;
		}
		
		static decltype(auto) GetFlags(const TString& pName)
		{
			using namespace entt::literals;
			auto _ = ECF::ComponentFlagsNone;
			entt::resolve(GetNameHash(pName)).func("get_flags"_hs).invoke({}, entt::forward_as_meta(_));
			return _;
		}
	};


	struct CComponentBase
	{
		virtual void OnConstruct(CEntity* pOwningEntity){};
		virtual void DrawPropertiesUI(CEntity& pEntity) {};
		virtual void Serialize(YAML::Emitter& pOut) {};
		virtual void Deserialize(YAML::Node& pData) {};

		static constexpr ECF::EComponentFlags Flags = ECF::DefaultComponentFlags;
	};
}