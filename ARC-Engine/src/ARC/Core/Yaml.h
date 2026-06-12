#pragma once
#pragma warning(push, 0)
#include "yaml-cpp/yaml.h"
#pragma warning(pop)
#include "ARC/Types/Color.h"
#include "ARC/Types/Vector.h"

namespace YAML
{
	template<>
	struct convert<arc::vec2>
	{
		static Node encode(const arc::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);

			node.set_style(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, arc::vec2& rhs) {
			if (!node.is_sequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<arc::vec3>
	{
		static Node encode(const arc::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);

			node.set_style(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, arc::vec3& rhs) {
			if (!node.is_sequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<arc::vec4>
	{
		static Node encode(const arc::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);

			node.set_style(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, arc::vec4& rhs) {
			if (!node.is_sequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<arc::color4>
	{
		static Node encode(const arc::color4& rhs) {
			Node node;
			node.push_back(rhs.r);
			node.push_back(rhs.g);
			node.push_back(rhs.b);
			node.push_back(rhs.a);

			node.set_style(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, arc::color4& rhs) {
			if (!node.is_sequence() || node.size() != 4)
				return false;

			rhs.r = node[0].as<float>();
			rhs.g = node[1].as<float>();
			rhs.b = node[2].as<float>();
			rhs.a = node[3].as<float>();
			return true;
		}
	};
}
namespace arc {
	template <typename T_>
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const T_& rhs) {
		out << YAML::convert<T_>::encode(rhs);
		return out;
	}
}