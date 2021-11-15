#pragma once

#include "ARC/Window.h"
#include "GLFW/glfw3.h"

namespace ARC{
	class WindowsWindow : public Window
	 {
		/*--------------FUNCS--------------*/
		public:
			WindowsWindow(const WindowProps& _props);
			virtual ~WindowsWindow();

			void OnUpdate() override;

			inline unsigned int GetWidth() const override { return m_Data.Width; }
			inline unsigned int GetHeight() const override { return m_Data.Height; }

			inline void SetEventCallback(const EventCallbackFn& _callback) override {
				m_Data.EventCallback = _callback;
			}
			virtual void SetVSync(bool _bEnabled) override;
			virtual bool IsVSync() const override {return m_Data.bVSync;}

		protected:
		private:
			virtual void Init(const WindowProps& _props);
			virtual void Shutdown();
		/*---------------------------------*/
	
		/*---------------VARS--------------*/
		public:
		protected:
		private:
			GLFWwindow* m_Window;

			struct WindowData
			{
				std::string Title;
				unsigned int Width, Height;
				unsigned int bVSync : 1;

				EventCallbackFn EventCallback; 
			};
			WindowData m_Data;

		/*---------------------------------*/
	};
}