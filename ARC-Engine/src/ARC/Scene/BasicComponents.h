#pragma once
#include "Component.h"
#include "ARC/Core/UUID.h"

namespace YAML { class Node; }
namespace YAML { class Emitter; }

namespace arc {
	struct transform_2d_component : public component_base
	{
		transform_2d Transform;

		transform_2d_component() = default;
		transform_2d_component(const transform_2d_component&) = default;
		transform_2d_component(const transform_2d& pTransform) : Transform(pTransform) {};

		virtual void draw_properties_ui(entity& pEntity) override;
		virtual void serialize(YAML::Emitter& pOut) override;
		virtual void deserialize(YAML::Node& pData) override;
		virtual u32 get_flags() override { return Flags; };

		static inline u32 Flags = ecf::DefaultComponentFlags | ecf::Serializable;

		operator transform_2d& () { return Transform; }
		operator const transform_2d& () const { return Transform; }
	};

	struct sprite_renderer_component : public component_base
	{
		color4 Color = color4::White();
		ref<texture_2d> Texture = nullptr;
		vec2 TextureScaling = vec2::OneVector();

		sprite_renderer_component() = default;
		sprite_renderer_component(const sprite_renderer_component&) = default;
		sprite_renderer_component(const color4& pColor, const ref<texture_2d>& pTex, const vec2& pTexScaling) : Color(pColor), Texture(pTex), TextureScaling(pTexScaling) {};

		virtual void draw_properties_ui(entity& pEntity) override;
		virtual void serialize(YAML::Emitter& pOut) override;
		virtual void deserialize(YAML::Node& pData) override;
		virtual u32 get_flags() override { return Flags; };

		static inline u32 Flags = ecf::DefaultComponentFlags | ecf::Serializable;
	};

	struct circle_renderer_component : public component_base
	{
		color4 Color = color4::White();
		float Thickness = 1.0f;
		float Sharpness = 0.995f;

		circle_renderer_component() = default;
		circle_renderer_component(const circle_renderer_component&) = default;

		virtual void draw_properties_ui(entity& pEntity) override;
		virtual void serialize(YAML::Emitter& pOut) override;
		virtual void deserialize(YAML::Node& pData) override;
		virtual u32 get_flags() override { return Flags; };

		static inline u32 Flags = ecf::DefaultComponentFlags | ecf::Serializable;
	};

	struct id_component : public component_base
	{
		uuid ID = uuid::generate();

		id_component() = default;
		id_component(const id_component& p) = default;
		id_component(const uuid& pID) : ID(pID) {};

		virtual u32 get_flags() override { return Flags; };
		static inline u32 Flags = ecf::ComponentFlagsNone; // using custom serialisation ecf::Serializable;

		operator auto& () { return ID; }
		operator const auto& () const { return ID; }
	};

	struct name_component : public component_base
	{
		string Name;

		name_component() = default;
		name_component(const name_component&) = default;
		name_component(const string& pName) : Name(pName) {};

		virtual void draw_properties_ui(entity& pEntity) override;
		virtual void serialize(YAML::Emitter& pOut) override;
		virtual void deserialize(YAML::Node& pData) override;
		virtual u32 get_flags() override { return Flags; };

		static inline u32 Flags = ecf::ShowInPropertiesPanel | ecf::Serializable;

		operator auto& () { return Name; }
		operator const auto& () const { return Name; }
	};

	struct camera_component : public component_base
	{
		scene_camera Camera;
		virtual ~camera_component() = default;

		uint8_t Primary : 1;
		uint8_t FixedAspectRatio : 1;

		camera_component() = default;
		camera_component(const camera_component&) = default;

		virtual void draw_properties_ui(entity& pEntity) override;
		virtual void serialize(YAML::Emitter& pOut) override;
		virtual void deserialize(YAML::Node& pData) override;
		virtual u32 get_flags() override { return Flags; };

		static inline u32 Flags = ecf::DefaultComponentFlags | ecf::Serializable;

		operator auto& () { return Camera; }
		operator const auto& () const { return Camera; }
	};

	struct native_script_component : public component_base
	{
		entity* OwningEntity = nullptr;
		entity_controller* Controller = nullptr;

		delegate<entity_controller* ()> InstantiateDelegate;
		delegate<void(entity_controller*&)> DestroyInstancesDelegate;

		virtual void on_construct(entity& pOwningEntity) override;
		virtual u32 get_flags() override { return Flags; };

		static inline u32 Flags = ecf::ComponentFlagsNone;

		template<typename T>
		void BindController()
		{
			InstantiateDelegate.bind([]() { return new T(); });
			DestroyInstancesDelegate.bind([](entity_controller*& pInst) { delete pInst; pInst = nullptr; });
		}
	};

	struct rigid_body_2d_component : public component_base
	{
		enum class body_type { Static = 0, Kinematic, Dynamic };
		body_type Type = body_type::Static;
		bool bFixedRotation;

		void* RuntimeBody = nullptr;

		rigid_body_2d_component() = default;
		rigid_body_2d_component(const rigid_body_2d_component&) = default;

		virtual void draw_properties_ui(entity& pEntity) override;
		virtual void serialize(YAML::Emitter& pOut) override;
		virtual void deserialize(YAML::Node& pData) override;
		virtual u32 get_flags() override { return Flags; };

		static inline u32 Flags = ecf::DefaultComponentFlags | ecf::Serializable;
	};

	struct box_collider_2d_component : public component_base
	{
		vec2 Offset = vec2::ZeroVector();
		vec2 size = vec2::OneVector()*0.5f;

		float Density=1.f;
		float Friction=0.5f;
		float Restitution=0.0f;
		float RestitutionThreshhold=0.5f;

		void* RuntimeFixture = nullptr;

		box_collider_2d_component() = default;
		box_collider_2d_component(const box_collider_2d_component&) = default;

		virtual void draw_properties_ui(entity& pEntity) override;
		virtual void serialize(YAML::Emitter& pOut) override;
		virtual void deserialize(YAML::Node& pData) override;
		virtual u32 get_flags() override { return Flags; };

		static inline u32 Flags = ecf::DefaultComponentFlags | ecf::Serializable;
	};

	struct circle_collider_2d_component : public component_base
	{
		vec2 Offset = vec2::ZeroVector();
		float Radius = 0.5f;

		float Density = 1.f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshhold = 0.5f;

		void* RuntimeFixture = nullptr;

		circle_collider_2d_component() = default;
		circle_collider_2d_component(const circle_collider_2d_component&) = default;

		virtual void draw_properties_ui(entity& pEntity) override;
		virtual void serialize(YAML::Emitter& pOut) override;
		virtual void deserialize(YAML::Node& pData) override;
		virtual u32 get_flags() override { return Flags; };

		static inline u32 Flags = ecf::DefaultComponentFlags | ecf::Serializable;
	};
}