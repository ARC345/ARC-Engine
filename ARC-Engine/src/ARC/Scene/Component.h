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

		static inline uint32_t Flags = ECF::DefaultComponentFlags;
		static inline int32_t OnTopPriority = 0; // >1 top priority, <1 means bottom priority in scene heirachy panel @TODO implemeant
	};
}