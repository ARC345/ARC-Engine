#pragma once
#include <string>

namespace arc {
	class event;

	class ARC_API layer
	{
	public:
		layer(const std::string& _name="Layer");
		virtual ~layer();

		virtual void on_attach(){}
		virtual void on_detach(){}
		virtual void on_update(float delta_time){}
		virtual void on_gui_render(){}
		virtual void on_event(event& _event){}

		inline const std::string& get_name() const { return debug_name_; }
	protected:
		std::string debug_name_;
	};
}