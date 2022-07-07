#include "arc_pch.h"
#include "Component.h"
#include "Entity.h"
#include "imgui.h"
#include "Scene.h"
#include "ARC/Core/Yaml.h"

namespace YAML {
	template<>
	struct convert<ARC::FVec2>
	{
		static Node encode(const ARC::FVec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}
		static bool decode(const Node& node, ARC::FVec2& rhs) {
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<ARC::FVec3>
	{
		static Node encode(const ARC::FVec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, ARC::FVec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<ARC::FVec4>
	{
		static Node encode(const ARC::FVec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, ARC::FVec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<ARC::FColor>
	{
		static Node encode(const ARC::FColor& rhs) {
			Node node;
			node.push_back(rhs.r);
			node.push_back(rhs.g);
			node.push_back(rhs.b);
			node.push_back(rhs.a);
			return node;
		}
		static bool decode(const Node& node, ARC::FColor& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.r = node[0].as<float>();
			rhs.g = node[1].as<float>();
			rhs.b = node[2].as<float>();
			rhs.a = node[3].as<float>();
			return true;
		}
	};
}
namespace ARC
{
	YAML::Emitter& operator << (YAML::Emitter& pOut, const FVec2& _) {
		pOut << YAML::Flow;
		pOut << YAML::BeginSeq << _.x << _.y << YAML::EndSeq;
		return pOut;
	}

	YAML::Emitter& operator << (YAML::Emitter& pOut, const FVec3& _) {
		pOut << YAML::Flow;
		pOut << YAML::BeginSeq << _.x << _.y << _.z << YAML::EndSeq;
		return pOut;
	}
	YAML::Emitter& operator << (YAML::Emitter& pOut, const FVec4& _) {
		pOut << YAML::Flow;
		pOut << YAML::BeginSeq << _.x << _.y << _.z << _.w << YAML::EndSeq;
		return pOut;
	}
	YAML::Emitter& operator << (YAML::Emitter& pOut, const FColor& _) {
		pOut << YAML::Flow;
		pOut << YAML::BeginSeq << _.r << _.g << _.b << _.a << YAML::EndSeq;
		return pOut;
	}

	void CNativeScriptComponent::OnConstruct(CEntity* pOwningEntity)
	{
		OwningEntity = pOwningEntity;
	}

	void CTransform2DComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		Transform.Location.DrawGuiControl("Translation", 100.f, FVec3::ZeroVector);
		FVec1 _2 = Math::Degrees(Transform.Rotation);
		_2.DrawGuiControl("Rotation", 100.f);
		Transform.Rotation = Math::Radians(_2.x);
		Transform.Scale.DrawGuiControl("Scale", 100.f, FVec2::OneVector);
	}

	void CTransform2DComponent::Serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Location" << YAML::Value << Transform.Location <<
			YAML::Key << "Rotation" << YAML::Value << Transform.Rotation <<
			YAML::Key << "Scale" << YAML::Value << Transform.Scale;
	}

	void CTransform2DComponent::Deserialize(YAML::Node& pData)
	{
		Transform.Location = pData["Location"].as<FVec3>();
		Transform.Rotation = pData["Rotation"].as<float>();
		Transform.Scale = pData["Scale"].as<FVec2>();
	}

	void CCameraComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		static const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
		const char* currentProjectionTypeString = projectionTypeStrings[(int)Camera.GetProjectionType()];

		if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
				if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
				{
					currentProjectionTypeString = projectionTypeStrings[i];
					Camera.SetProjectionType(CSceneCamera::EProjectionType(i));
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if (Camera.GetProjectionType() == CSceneCamera::EProjectionType::Orthographic)
		{
			float orthoSize = Camera.GetOrthographicSize();
			if (ImGui::DragFloat("Size", &orthoSize))
				Camera.SetOrthographicSize(orthoSize);

			float orthoNearClip = Camera.GetOrthographicNearClip();
			if (ImGui::DragFloat("NearClip", &orthoNearClip))
				Camera.SetOrthographicNearClip(orthoNearClip);

			float orthoFarClip = Camera.GetOrthographicFarClip();
			if (ImGui::DragFloat("FarClip", &orthoFarClip))
				Camera.SetOrthographicFarClip(orthoFarClip);
		}
		if (Camera.GetProjectionType() == CSceneCamera::EProjectionType::Perspective)
		{
			float perspectiveFOV = Math::Conv<ERotType::Radians, ERotType::Degrees>(Camera.GetPerspectiveFOV());
			if (ImGui::DragFloat("FOV", &perspectiveFOV))
				Camera.SetPerspectiveFOV(perspectiveFOV);

			float perspectiveNearClip = Camera.GetPerspectiveNearClip();
			if (ImGui::DragFloat("NearClip", &perspectiveNearClip))
				Camera.SetPerspectiveNearClip(perspectiveNearClip);

			float perspectiveFarClip = Camera.GetPerspectiveFarClip();
			if (ImGui::DragFloat("FarClip", &perspectiveFarClip))
				Camera.SetPerspectiveFarClip(perspectiveFarClip);
		}
	}

	void CCameraComponent::Serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Primary" << YAML::Value << bool(bPrimary) <<
		YAML::Key << "FixedAspectRatio" << YAML::Value << bool(bFixedAspectRatio) <<
		YAML::Key << "ProjectionType" << YAML::Value << (int)Camera.GetProjectionType() <<
			YAML::Key << "Perspective" << YAML::Value << YAML::BeginMap <<
				YAML::Key << "FOV" << YAML::Value  << Camera.GetPerspectiveFOV() <<
				YAML::Key << "NearClip" << YAML::Value  << Camera.GetPerspectiveNearClip() <<
				YAML::Key << "FarClip" << YAML::Value  << Camera.GetPerspectiveFarClip() <<
				YAML::EndMap <<
			YAML::Key << "Orthographic" << YAML::Value << YAML::BeginMap <<
				YAML::Key << "Size" << YAML::Value  << Camera.GetOrthographicSize() <<
				YAML::Key << "NearClip" << YAML::Value  << Camera.GetOrthographicNearClip() <<
				YAML::Key << "FarClip" << YAML::Value  << Camera.GetOrthographicFarClip() <<
				YAML::EndMap;
	}

	void CCameraComponent::Deserialize(YAML::Node& pData)
	{
		bPrimary = pData["Primary"].as<bool>();
		bFixedAspectRatio = pData["FixedAspectRatio"].as<bool>();

		Camera.SetProjectionType(CSceneCamera::EProjectionType(pData["ProjectionType"].as<int>()));
		
		Camera.SetOrthographicSize(pData["Orthographic"]["Size"].as<float>());
		Camera.SetOrthographicNearClip(pData["Orthographic"]["NearClip"].as<float>());
		Camera.SetOrthographicFarClip(pData["Orthographic"]["FarClip"].as<float>());
		
		Camera.SetPerspectiveFOV(pData["Perspective"]["FOV"].as<float>());
		Camera.SetPerspectiveNearClip(pData["Perspective"]["NearClip"].as<float>());
		Camera.SetPerspectiveFarClip(pData["Perspective"]["FarClip"].as<float>());
	}

	void CNameComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), Name.c_str());
		if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			Name = buffer;
	}

	void CNameComponent::Serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Name" << YAML::Value << Name;
	}

	void CNameComponent::Deserialize(YAML::Node& pData)
	{
		Name = pData["Name"].as<TString>();
	}

	void CSpriteRendererComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		ImGui::ColorEdit4("Color", Color.Data());
	}

	void CSpriteRendererComponent::Serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Color" << YAML::Value << Color;
	}

	void CSpriteRendererComponent::Deserialize(YAML::Node& pData)
	{
		Color = pData["Color"].as<FColor>();
	}

	void CElectricSignComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		static const char* SignStrings[] = { "Negative", "Neutral", "Positive"};
		const char* currentSignString = SignStrings[1+Sign];

		if (ImGui::BeginCombo("Sign", currentSignString))
		{
			for (int i = 0; i < 3; i++)
			{
				bool isSelected = currentSignString == SignStrings[i];
				if (ImGui::Selectable(SignStrings[i], isSelected))
				{
					currentSignString = SignStrings[i];
					Sign = i-1;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

	}

	void CElectricSignComponent::Serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Sign" << YAML::Value << Sign;
	}

	void CElectricSignComponent::Deserialize(YAML::Node& pData)
	{
		Sign = pData["Sign"].as<int8_t>();
	}

	void CVelocityComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		ImGui::DragFloat3("Velocity", Velocity.Data());
	}

	void CVelocityComponent::Serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Velocity" << YAML::Value << Velocity;
	}

	void CVelocityComponent::Deserialize(YAML::Node& pData)
	{
		Velocity = pData["Velocity"].as<FVec3>();
	}

	void CMassComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		ImGui::DragFloat("Mass", &Mass);
	}

	void CMassComponent::Serialize(YAML::Emitter & pOut)
	{
		pOut << YAML::Key << "Mass" << YAML::Value << Mass;
	}

	void CMassComponent::Deserialize(YAML::Node & pData)
	{
		Mass = pData["Mass"].as<float>();
	}

}
