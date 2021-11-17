#pragma once

#include "ARC/Core.h"

namespace ARC { class CEvent; }

namespace ARC {
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& _title = "ARC-Engine", unsigned int _width = 1280, unsigned int _height = 720) : Title(_title), Width(_width), Height(_height){}
	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(CEvent&)>;

		virtual ~Window() {}
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& _callback) = 0;
		virtual void SetVSync(bool _bEnabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& _props = WindowProps());
	};
}