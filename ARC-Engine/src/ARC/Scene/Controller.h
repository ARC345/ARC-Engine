#pragma once

namespace arc { class entity; }

namespace arc
{
	class entity_controller
	{
	public:
		virtual ~entity_controller(){};

		inline bool is_setup() const { return controlled_entity_; }
		entity* get_controlled_entity();

		virtual void set_up(entity* pEntity);

		virtual void on_create() {};
		virtual void OnDestroy() {};
		virtual void on_update(float pDeltaTime) {};
	private:
		entity* controlled_entity_;
		friend class scene;
	};

	class camera_controller : public entity_controller
	{
	public:
		camera_controller() {};

		virtual void on_create() override;
		virtual void OnDestroy() override;
		virtual void on_update(float pDeltaTime) override;
	private:
	};
}