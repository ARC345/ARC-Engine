#pragma once

#include "ARC/Core/Window.h"
#pragma warning(push, 0)
#include "GLFW/glfw3.h"
#pragma warning(pop)

namespace arc { class graphics_context; }

namespace arc{
	class windows_window : public window
	 {
		/*--------------FUNCS--------------*/
		public:
			windows_window(const window_props& _props);
			virtual ~windows_window();

			void on_update() override;

			inline unsigned int get_width() const override { return data_.Width; }
			inline unsigned int get_height() const override { return data_.Height; }
			inline void set_width(unsigned int _) override {data_.Width = _; };
			inline void set_height(unsigned int _) override { data_.Height = _; };

			inline void set_event_callback(const event_callback_fn& _callback) override { data_.EventCallback = _callback; }

			virtual void set_vsync(bool _bEnabled) override;
			virtual bool is_vsync() const override {return data_.bVSync;}

			virtual void* get_native_window() const override;

	 protected:
		private:
			virtual void init(const window_props& _props);
			virtual void shutdown();
		/*---------------------------------*/
	
		/*---------------VARS--------------*/
		public:
		protected:
		private:
			GLFWwindow* window_;
			graphics_context* context_;
			struct window_data
			{
				std::string Title;
				unsigned int Width, Height;
				unsigned int bVSync : 1;

				event_callback_fn EventCallback; 
			};
			window_data data_;

		/*---------------------------------*/
	};
}