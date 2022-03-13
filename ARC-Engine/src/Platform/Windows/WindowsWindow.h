#pragma once

#include "ARC/Core/Window.h"
#include "GLFW/glfw3.h"

namespace ARC { class CGraphicsContext; }

namespace ARC{
	class CWindowsWindow : public CWindow
	 {
		/*--------------FUNCS--------------*/
		public:
			CWindowsWindow(const SWindowProps& _props);
			virtual ~CWindowsWindow();

			void OnUpdate() override;

			inline unsigned int GetWidth() const override { return m_Data.Width; }
			inline unsigned int GetHeight() const override { return m_Data.Height; }
			inline void SetWidth(unsigned int _) override {m_Data.Width = _; };
			inline void SetHeight(unsigned int _) override { m_Data.Height = _; };

			inline void SetEventCallback(const EventCallbackFn& _callback) override { m_Data.EventCallback = _callback; }

			virtual void SetVSync(bool _bEnabled) override;
			virtual bool IsVSync() const override {return m_Data.bVSync;}

			virtual void* GetNativeWindow() const override;

	 protected:
		private:
			virtual void Init(const SWindowProps& _props);
			virtual void Shutdown();
		/*---------------------------------*/
	
		/*---------------VARS--------------*/
		public:
		protected:
		private:
			GLFWwindow* m_Window;
			CGraphicsContext* m_Context;
			struct SWindowData
			{
				std::string Title;
				unsigned int Width, Height;
				unsigned int bVSync : 1;

				EventCallbackFn EventCallback; 
			};
			SWindowData m_Data;

		/*---------------------------------*/
	};
}