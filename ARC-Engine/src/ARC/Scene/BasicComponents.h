#pragma once
#include "Component.h"

namespace YAML { class Node; }
namespace YAML { class Emitter; }

namespace ARC {
	struct CTransform2DComponent : public CComponentBase
	{
		FTransform2D Transform;

		CTransform2DComponent() = default;
		CTransform2DComponent(const CTransform2DComponent&) = default;
		CTransform2DComponent(const FTransform2D& pTransform) : Transform(pTransform) {};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;

		operator FTransform2D& () { return Transform; }
		operator const FTransform2D& () const { return Transform; }
	};

	struct CSpriteRendererComponent : public CComponentBase
	{
		FColor4 Color = FColor4::White();
		TRef<CTexture2D> Texture = nullptr;
		FVec2 TextureScaling = FVec2::OneVector();

		CSpriteRendererComponent() = default;
		CSpriteRendererComponent(const CSpriteRendererComponent&) = default;
		CSpriteRendererComponent(const FColor4& pColor, const TRef<CTexture2D>& pTex, const FVec2& pTexScaling) : Color(pColor), Texture(pTex), TextureScaling(pTexScaling) {};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};

	struct CNameComponent : public CComponentBase
	{
		TString Name;

		CNameComponent() = default;
		CNameComponent(const CNameComponent&) = default;
		CNameComponent(const TString& pName) : Name(pName) {};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::ShowInPropertiesPanel | ECF::Serializable;

		operator auto& () { return Name; }
		operator const auto& () const { return Name; }
	};

	struct CCameraComponent : public CComponentBase
	{
		CSceneCamera Camera;
		virtual ~CCameraComponent() = default;

		uint8_t bPrimary : 1;
		uint8_t bFixedAspectRatio : 1;

		CCameraComponent() = default;
		CCameraComponent(const CCameraComponent&) = default;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;

		operator auto& () { return Camera; }
		operator const auto& () const { return Camera; }
	};

	struct CNativeScriptComponent : public CComponentBase
	{
		CEntity* OwningEntity = nullptr;
		CEntityController* Controller = nullptr;

		TDelegate<CEntityController* ()> InstantiateDelegate;
		TDelegate<void(CEntityController*&)> DestroyInstancesDelegate;

		virtual void OnConstruct(CEntity& pOwningEntity) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = 0;

		template<typename T>
		void BindController()
		{
			InstantiateDelegate.Bind([]() { return new T(); });
			DestroyInstancesDelegate.Bind([](CEntityController*& pInst) { delete pInst; pInst = nullptr; });
		}
	};
	struct CMassComponent : public CComponentBase
	{
		float Mass = 1;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};
	struct CVelocityComponent : public CComponentBase
	{
		FVec3 Velocity = FVec3::ZeroVector();

		CVelocityComponent() = default;
		CVelocityComponent(const CVelocityComponent&) = default;
		CVelocityComponent(const FVec3& pVelocity) : Velocity(pVelocity) {};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};
	struct CAngularVelocityComponent : public CComponentBase
	{
		FVec3 AngularVelocity = FVec3::ZeroVector();

		CAngularVelocityComponent() = default;
		CAngularVelocityComponent(const CAngularVelocityComponent&) = default;
		CAngularVelocityComponent(const FVec3& pAngularVelocity) : AngularVelocity(pAngularVelocity) {};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};
	struct CAccelerationComponent : public CComponentBase
	{
		FVec3 Acceleration = FVec3::ZeroVector();

		CAccelerationComponent() = default;
		CAccelerationComponent(const CAccelerationComponent&) = default;
		CAccelerationComponent(const FVec3& pAcceleration) : Acceleration(pAcceleration) {};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};
	struct CAngularAccelerationComponent : public CComponentBase
	{
		FVec3 AngularAcceleration = FVec3::ZeroVector();

		CAngularAccelerationComponent() = default;
		CAngularAccelerationComponent(const CAngularAccelerationComponent&) = default;
		CAngularAccelerationComponent(const FVec3& pAngularAcceleration) : AngularAcceleration(pAngularAcceleration) {};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};
}