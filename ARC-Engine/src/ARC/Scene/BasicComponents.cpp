#include "arc_pch.h"
#include "BasicComponents.h"
#include "Component.h"
#include "Entity.h"
#include "imgui.h"
#include "Scene.h"
#include "ARC/Core/Yaml.h"
#include "ARC/GUI/ImGuiHelper.h"
#include "ARC/Core/PlatformUtils.h"

namespace arc
{
	void native_script_component::on_construct(entity& pOwningEntity)
	{
		OwningEntity = &pOwningEntity;
	}

	void transform_2d_component::draw_properties_ui(entity& pEntity)
	{
		gui_helper::draw_gui_control(Transform.Location, "Translation", 100.f, vec3::ZeroVector());
		vec1 _ = math::Degrees(Transform.Rotation);
		gui_helper::draw_gui_control(_, "Rotation", 100.f);
		Transform.Rotation = math::Radians(_.x);
		gui_helper::draw_gui_control(Transform.Scale, "Scale", 100.f, vec2::OneVector());
	}

	void transform_2d_component::serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Location" << YAML::Value << Transform.Location <<
			YAML::Key << "Rotation" << YAML::Value << Transform.Rotation <<
			YAML::Key << "Scale" << YAML::Value << Transform.Scale;
	}

	void transform_2d_component::deserialize(YAML::Node& pData)
	{
		Transform.Location = pData["Location"].as<vec3>();
		Transform.Rotation = pData["Rotation"].as<float>();
		Transform.Scale = pData["Scale"].as<vec2>();
	}

	void camera_component::draw_properties_ui(entity& pEntity)
	{
		bool bprimary = Primary;
		ImGui::Checkbox("PrimaryCamera", &bprimary);
		Primary = bprimary;  
		static const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
		const char* currentProjectionTypeString = projectionTypeStrings[(int)Camera.get_projection_type()];

		if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
				if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
				{
					currentProjectionTypeString = projectionTypeStrings[i];
					Camera.set_projection_type(scene_camera::projection_type(i));
				}
				if (isSelected)
					ImGui::set_item_default_focus();
			}

			ImGui::EndCombo();
		}

		if (Camera.get_projection_type() == scene_camera::projection_type::Orthographic)
		{
			float orthoSize = Camera.get_orthographic_size();
			if (ImGui::DragFloat("size", &orthoSize))
				Camera.set_orthographic_size(orthoSize);

			float orthoNearClip = Camera.get_orthographic_near_clip();
			if (ImGui::DragFloat("NearClip", &orthoNearClip))
				Camera.set_orthographic_near_clip(orthoNearClip);

			float orthoFarClip = Camera.get_orthographic_far_clip();
			if (ImGui::DragFloat("FarClip", &orthoFarClip))
				Camera.set_orthographic_far_clip(orthoFarClip);
		}
		if (Camera.get_projection_type() == scene_camera::projection_type::Perspective)
		{
			float perspectiveFOV = math::Conv<rot_type::Radians, rot_type::Degrees>(Camera.get_perspective_fov());
			if (ImGui::DragFloat("FOV", &perspectiveFOV))
				Camera.set_perspective_fov(perspectiveFOV);

			float perspectiveNearClip = Camera.get_perspective_near_clip();
			if (ImGui::DragFloat("NearClip", &perspectiveNearClip))
				Camera.set_perspective_near_clip(perspectiveNearClip);

			float perspectiveFarClip = Camera.get_perspective_far_clip();
			if (ImGui::DragFloat("FarClip", &perspectiveFarClip))
				Camera.set_perspective_far_clip(perspectiveFarClip);
		}
	}

	void camera_component::serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Primary" << YAML::Value << bool(Primary) <<
		YAML::Key << "FixedAspectRatio" << YAML::Value << bool(FixedAspectRatio) <<
		YAML::Key << "ProjectionType" << YAML::Value << (int)Camera.get_projection_type() <<
			YAML::Key << "Perspective" << YAML::Value << YAML::BeginMap <<
				YAML::Key << "FOV" << YAML::Value  << Camera.get_perspective_fov() <<
				YAML::Key << "NearClip" << YAML::Value  << Camera.get_perspective_near_clip() <<
				YAML::Key << "FarClip" << YAML::Value  << Camera.get_perspective_far_clip() <<
				YAML::EndMap <<
			YAML::Key << "Orthographic" << YAML::Value << YAML::BeginMap <<
				YAML::Key << "size" << YAML::Value  << Camera.get_orthographic_size() <<
				YAML::Key << "NearClip" << YAML::Value  << Camera.get_orthographic_near_clip() <<
				YAML::Key << "FarClip" << YAML::Value  << Camera.get_orthographic_far_clip() <<
				YAML::EndMap;
	}

	void camera_component::deserialize(YAML::Node& pData)
	{
		Primary = pData["Primary"].as<bool>();
		FixedAspectRatio = pData["FixedAspectRatio"].as<bool>();

		Camera.set_projection_type(scene_camera::projection_type(pData["ProjectionType"].as<int>()));
		
		Camera.set_orthographic_size(pData["Orthographic"]["size"].as<float>());
		Camera.set_orthographic_near_clip(pData["Orthographic"]["NearClip"].as<float>());
		Camera.set_orthographic_far_clip(pData["Orthographic"]["FarClip"].as<float>());
		
		Camera.set_perspective_fov(pData["Perspective"]["FOV"].as<float>());
		Camera.set_perspective_near_clip(pData["Perspective"]["NearClip"].as<float>());
		Camera.set_perspective_far_clip(pData["Perspective"]["FarClip"].as<float>());
	}

	void name_component::draw_properties_ui(entity& pEntity)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), Name.c_str());
		if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			Name = buffer;
	}

	void name_component::serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Name" << YAML::Value << Name;
	}

	void name_component::deserialize(YAML::Node& pData)
	{
		Name = pData["Name"].as<string>();
	}

	void sprite_renderer_component::draw_properties_ui(entity& pEntity)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		if (Texture)
			strcpy_s(buffer, Texture->get_path().string().c_str());
		gui_helper::draw_gui_control(Color, "Color", 100.f, color4::Black());
		
		ImGui::InputText("TexturePath   ", buffer, 256);
		if (ImGui::is_item_deactivated_after_edit() && 
		   (!Texture || Texture->get_path() != buffer))
		{
			Texture = texture_2d::create(std::filesystem::path(buffer));
		}
		
		if (ImGui::BeginDragDropTarget())
		{
			if (const auto* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const auto* path = (const wchar_t*)payload->Data;
				Texture = texture_2d::create(std::filesystem::path("assets") / std::filesystem::path(path)); // todo replace assets path 
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SameLine();
		if(ImGui::Button("...")) {
			auto filepath = file_dialogs::open_file("ARC-Engine Texture (*.png)\0*.png\0");
			if (!filepath.empty())
			{
				Texture = texture_2d::create(filepath);
			}
		}
		gui_helper::draw_gui_control(TextureScaling, "Scaling", 100.f, vec2::OneVector());
	}

	void sprite_renderer_component::serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Color" << YAML::Value << Color;
		if (Texture)
			pOut << 
			YAML::Key << "TexturePath" << YAML::Value << Texture->get_path().string() <<
			YAML::Key << "TextureScaling" << YAML::Value << TextureScaling;
	}

	void sprite_renderer_component::deserialize(YAML::Node& pData)
	{
		Color = pData["Color"].as<color4>();
		if (pData["TexturePath"])
		{
			string texturePath = pData["TexturePath"].as<string>();
			Texture = texture_2d::create(texturePath);
			TextureScaling = pData["TextureScaling"].as<vec2>();
		}
	}

	void box_collider_2d_component::draw_properties_ui(entity& pEntity)
	{
		ImGui::DragFloat2("Offset", Offset.Data());
		ImGui::DragFloat2("size", size.Data());
		ImGui::DragFloat("Density", &Density);
		ImGui::DragFloat("Friction", &Friction);
		ImGui::DragFloat("Restitution", &Restitution);
		ImGui::DragFloat("RestitutionThreshhold", &RestitutionThreshhold);
	}

	void box_collider_2d_component::serialize(YAML::Emitter& pOut)
	{
		pOut 
			<< YAML::Key << "Offset" << YAML::Value << Offset
			<< YAML::Key << "size" << YAML::Value << size
			<< YAML::Key << "Density" << YAML::Value << Density
			<< YAML::Key << "Friction" << YAML::Value << Friction
			<< YAML::Key << "Restitution" << YAML::Value << Restitution
			<< YAML::Key << "RestitutionThreshhold" << YAML::Value << RestitutionThreshhold;
	}

	void box_collider_2d_component::deserialize(YAML::Node& pData)
	{
		Offset = pData["Offset"].as<vec2>();
		size = pData["size"].as<vec2>();
		Density = pData["Density"].as<float>();
		Friction = pData["Friction"].as<float>();
		Restitution = pData["Restitution"].as<float>();
		RestitutionThreshhold = pData["RestitutionThreshhold"].as<float>();

	}

	void circle_collider_2d_component::draw_properties_ui(entity& pEntity)
	{
		ImGui::DragFloat2("Offset", Offset.Data());
		ImGui::DragFloat("Radius", &Radius);
		ImGui::DragFloat("Density", &Density);
		ImGui::DragFloat("Friction", &Friction);
		ImGui::DragFloat("Restitution", &Restitution);
		ImGui::DragFloat("RestitutionThreshhold", &RestitutionThreshhold);
	}

	void circle_collider_2d_component::serialize(YAML::Emitter& pOut)
	{
		pOut
			<< YAML::Key << "Offset" << YAML::Value << Offset
			<< YAML::Key << "Radius" << YAML::Value << Radius
			<< YAML::Key << "Density" << YAML::Value << Density
			<< YAML::Key << "Friction" << YAML::Value << Friction
			<< YAML::Key << "Restitution" << YAML::Value << Restitution
			<< YAML::Key << "RestitutionThreshhold" << YAML::Value << RestitutionThreshhold;
	}

	void circle_collider_2d_component::deserialize(YAML::Node& pData)
	{
		Offset = pData["Offset"].as<vec2>();
		Radius = pData["Radius"].as<float>();
		Density = pData["Density"].as<float>();
		Friction = pData["Friction"].as<float>();
		Restitution = pData["Restitution"].as<float>();
		RestitutionThreshhold = pData["RestitutionThreshhold"].as<float>();

	}

	void rigid_body_2d_component::draw_properties_ui(entity& pEntity)
	{
		static const char* bodyTypeStrings[] = { "Static", "Kinematic", "Dynamic" };
		const char* currentBodyTypeString = bodyTypeStrings[(int)Type];

		if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
		{
			for (int i = 0; i < 3; i++)
			{
				bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
				if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
				{
					currentBodyTypeString = bodyTypeStrings[i];
					Type = rigid_body_2d_component::body_type(i);
				}
				if (isSelected)
					ImGui::set_item_default_focus();
			}

			ImGui::EndCombo();
		}

		ImGui::Checkbox("Fixed Rotation", &bFixedRotation);
	}

	void rigid_body_2d_component::serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Type" << YAML::Value << (int)Type <<
			YAML::Key << "bFixedRotation" << YAML::Value << bool(bFixedRotation);
	}

	void rigid_body_2d_component::deserialize(YAML::Node& pData)
	{
		Type = (body_type)pData["Type"].as<int>();
		bFixedRotation = pData["bFixedRotation"].as<bool>();
	}

	void circle_renderer_component::draw_properties_ui(entity& pEntity)
	{
		gui_helper::draw_gui_control(Color, "Color", 100.f, color4::Black());
		ImGui::DragFloat("Thickness", &Thickness, 0.025f, 0.0f, 1.0f);
		ImGui::DragFloat("Sharpness", &Sharpness, 0.0005f, 0.0f, 1.0f);
	}

	void circle_renderer_component::serialize(YAML::Emitter& pOut)
	{
		pOut << 
			YAML::Key << "Color" << YAML::Value << Color <<
			YAML::Key << "Thickness" << YAML::Value << Thickness <<
			YAML::Key << "Sharpness" << YAML::Value << Sharpness;

	}

	void circle_renderer_component::deserialize(YAML::Node& pData)
	{
		Color = pData["Color"].as<color4>();
		Thickness = pData["Thickness"].as<float>();
		Sharpness = pData["Sharpness"].as<float>();
	}
}
