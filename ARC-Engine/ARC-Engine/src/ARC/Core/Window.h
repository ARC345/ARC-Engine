#pragma once

#include <functional>

namespace ARC { class CEvent; }

namespace ARC {
	struct SWindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		SWindowProps(const std::string& _title = "ARC-Engine", unsigned int _width = 1280, unsigned int _height = 720) : Title(_title), Width(_width), Height(_height){}
	};

	class CWindow {
	public:
		using EventCallbackFn = std::function<void(CEvent&)>;

		virtual ~CWindow() {}
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void SetWidth(unsigned int _) = 0;
		virtual void SetHeight(unsigned int _) = 0;
		
		virtual void SetEventCallback(const EventCallbackFn& _callback) = 0;
		virtual void SetVSync(bool _bEnabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static CWindow* Create(const SWindowProps& _props = SWindowProps());
	};
}