#pragma once
#include "Component.h"
#include "ARC/Core/UUID.h"

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
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::DefaultComponentFlags | ECF::Serializable;

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
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};

	struct CCircleRendererComponent : public CComponentBase
	{
		FColor4 Color = FColor4::White();
		float Thickness = 1.0f;
		float Sharpness = 0.995f;

		CCircleRendererComponent() = default;
		CCircleRendererComponent(const CCircleRendererComponent&) = default;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};

	struct CIDComponent : public CComponentBase
	{
		TUUID ID = SUUID::Generate();

		CIDComponent() = default;
		CIDComponent(const CIDComponent& p) = default;
		CIDComponent(const TUUID& pID) : ID(pID) {};

		virtual TUInt32 GetFlags() override { return Flags; };
		static inline TUInt32 Flags = ECF::ComponentFlagsNone; // using custom serialisation ECF::Serializable;

		operator auto& () { return ID; }
		operator const auto& () const { return ID; }
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
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::ShowInPropertiesPanel | ECF::Serializable;

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
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::DefaultComponentFlags | ECF::Serializable;

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
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::ComponentFlagsNone;

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
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};

	struct CRigidBody2DComponent : public CComponentBase
	{
		enum class EBodyType { Static = 0, Kinematic, Dynamic };
		EBodyType Type = EBodyType::Static;
		bool bFixedRotation;

		void* RuntimeBody = nullptr;

		CRigidBody2DComponent() = default;
		CRigidBody2DComponent(const CRigidBody2DComponent&) = default;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};

	struct CBoxCollider2DComponent : public CComponentBase
	{
		FVec2 Offset = FVec2::ZeroVector();
		FVec2 Size = FVec2::OneVector()*0.5f;

		float Density=1.f;
		float Friction=0.5f;
		float Restitution=0.0f;
		float RestitutionThreshhold=0.5f;

		void* RuntimeFixture = nullptr;

		CBoxCollider2DComponent() = default;
		CBoxCollider2DComponent(const CBoxCollider2DComponent&) = default;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};

	struct CCircleCollider2DComponent : public CComponentBase
	{
		FVec2 Offset = FVec2::ZeroVector();
		float Radius = 0.5f;

		float Density = 1.f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshhold = 0.5f;

		void* RuntimeFixture = nullptr;

		CCircleCollider2DComponent() = default;
		CCircleCollider2DComponent(const CCircleCollider2DComponent&) = default;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual TUInt32 GetFlags() override { return Flags; };

		static inline TUInt32 Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};
}