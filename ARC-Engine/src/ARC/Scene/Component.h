#pragma once

#include "ARC/Types/Transform2D.h"
#include "ARC/Types/Delegate.h"
#include "ARC/Types/Color.h"
#include "ARC/Renderer/Texture.h"
#include "ARC/Objects/Ecs.h"
#include "SceneCamera.h"
#include "Controller.h"

namespace YAML { class Emitter; }
namespace YAML { class Node; }

namespace ARC {
	class CEntity;

	#define CFlags(x) static constexpr uint32_t Flags = x;

	namespace ECF {
		enum EComponentFlags : uint32_t
		{
			ComponentFlagsNone = 0,
			ShowInPropertiesPanel = 1 << 0,
			AutoName = 1 << 1,
			ShowHeader = 1 << 2,
			AutoAddToAll = 1 << 3,
			Serializable = 1 << 4,
			DefaultComponentFlags = ShowInPropertiesPanel | AutoName | ShowHeader
		};
	}

	struct CComponentTraits
	{
		template<typename T>
		static constexpr const char* GetName()
		{
			if constexpr (T::Flags & ECF::AutoName)	return ECS::ComponentName<T>::Get()+1; // +1 to remove the first char which should be C ( CComponentName -> ComponentName )
			else return T::ComponentName;
		}
		template<typename T>
		static constexpr const char* GetFlags()
		{
			return T::Flags;
		}

	};

	struct CComponentBase
	{
		virtual void OnConstruct(CEntity* pOwningEntity){};
		virtual void DrawPropertiesUI(CEntity& pEntity) {};
		virtual void Serialize(YAML::Emitter& pOut) {};
		virtual void Deserialize(YAML::Node& pData) {};

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags;
		static constexpr const char* ComponentName = "";
	};
}