#pragma once
#include "ARC/Core/Application.h"

namespace arc {
	class editor_app : public arc::application
	{
	public:
		editor_app();
		~editor_app() {};

		inline virtual std::string get_app_name() override { return "ARC-Editor"; };
	};
}