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

	#define ARCComponent(x) static constexpr uint32_t Flags = x;

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

	struct CTransform2DComponent : public CComponentBase
	{
		FTransform2D Transform;

		CTransform2DComponent() = default;
		CTransform2DComponent(const CTransform2DComponent&) = default;
		CTransform2DComponent(const FTransform2D& pTransform) : Transform(pTransform){};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::AutoAddToAll | ECF::Serializable;

		operator FTransform2D& () { return Transform; }
		operator const FTransform2D& () const { return Transform; }
	};
	
	struct CSpriteRendererComponent : public CComponentBase
	{
		FColor Color = FColor::White;
		TRef<CTexture2D> Texture = nullptr;
		FVec2 TextureScaling = FVec2::OneVector;

		CSpriteRendererComponent() = default;
		CSpriteRendererComponent(const CSpriteRendererComponent&) = default;
		CSpriteRendererComponent(const FColor& pColor, const TRef<CTexture2D>& pTex, const FVec2& pTexScaling) : Color(pColor), Texture(pTex), TextureScaling(pTexScaling) {};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};
	
	struct CNameComponent : public CComponentBase
	{
		TString Name;

		CNameComponent() = default;
		CNameComponent(const CNameComponent&) = default;
		CNameComponent(const TString& pName) : Name(pName){};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;

		static constexpr uint32_t Flags = ECF::ShowInPropertiesPanel | ECF::AutoName | ECF::AutoAddToAll | ECF::Serializable;

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

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;

		operator auto& () { return Camera; }
		operator const auto& () const { return Camera; }
	};

	struct CNativeScriptComponent : public CComponentBase
	{
		CEntity* OwningEntity = nullptr;
		CEntityController* Controller = nullptr;

		CDelegate<CEntityController*()> InstantiateDelegate;
		CDelegate<void(CEntityController*&)> DestroyInstancesDelegate;

		virtual void OnConstruct(CEntity* pOwningEntity);

		static constexpr uint32_t Flags = ECF::AutoName;

		template<typename T>
		void BindController()
		{
			InstantiateDelegate.Bind([]() { return new T(); });
			DestroyInstancesDelegate.Bind([](CEntityController*& pInst) { delete pInst; pInst = nullptr; });
		}
	};

	//@TEMP
	struct CNetForceComponent : public CComponentBase
	{
		FVec3 NetForce = FVec3::ZeroVector;

		static constexpr uint32_t Flags = ECF::AutoName | ECF::AutoAddToAll;
	};
	struct CElectricSignComponent : public CComponentBase
	{
		enum EElectricSign { Negative = -1, Neutral = 0, Positive=1 };
		int8_t Sign = 0;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::AutoAddToAll | ECF::Serializable;
	};
	struct CMassComponent : public CComponentBase
	{
		float Mass = 1;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};
	struct CVelocityComponent : public CComponentBase
	{
		FVec3 Velocity = FVec3::ZeroVector;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable | ECF::AutoAddToAll;
	};

	RegisterComponent(CNameComponent);
	RegisterComponent(CTransform2DComponent);
	RegisterComponent(CNativeScriptComponent);
	RegisterComponent(CSpriteRendererComponent);
	RegisterComponent(CCameraComponent);
	RegisterComponent(CNetForceComponent);
	RegisterComponent(CElectricSignComponent);
	RegisterComponent(CMassComponent);
	RegisterComponent(CVelocityComponent);
}