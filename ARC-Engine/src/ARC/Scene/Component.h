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

namespace arc {
	class entity;

	namespace ecf {
		enum component_flags : uint32_t
		{
			ComponentFlagsNone = 0,
			ShowInPropertiesPanel = 1 << 0,
			ShowHeader = 1 << 1,
			Serializable = 1 << 2,
			DefaultComponentFlags = ShowInPropertiesPanel | ShowHeader
		};
	}

	struct component_base
	{
		virtual void on_construct(entity& pOwningEntity){};
		virtual void draw_properties_ui(entity& pEntity) {};
		virtual void serialize(YAML::Emitter& pOut) {};
		virtual void deserialize(YAML::Node& pData) {};
		virtual uint32_t get_flags()=0;

		static inline uint32_t Flags = ecf::DefaultComponentFlags;
		static inline int32_t OnTopPriority = 0; // >1 top priority, <1 means bottom priority in scene heirachy panel @TODO implemeant
	};
}