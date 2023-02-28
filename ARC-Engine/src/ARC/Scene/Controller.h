#pragma once

namespace ARC { class CEntity; }

namespace ARC
{
	class CEntityController
	{
	public:
		virtual ~CEntityController(){};

		inline bool IsSetup() const { return mControlledEntity; }
		CEntity* GetControlledEntity();

		virtual void Setup(CEntity* pEntity);

		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(float pDeltaTime) {};
	private:
		CEntity* mControlledEntity;
		friend class CScene;
	};

	class CCameraController : public CEntityController
	{
	public:
		CCameraController() {};

		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnUpdate(float pDeltaTime) override;
	private:
	};
}