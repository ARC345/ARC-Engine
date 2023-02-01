#pragma once

#include <functional>

namespace ARC { class CEvent; }

namespace ARC {
	struct SWindowProps {
		TString Title;
		uint32_t Width;
		uint32_t Height;

		SWindowProps(const TString& _title = "ARC-Engine", uint32_t _width = 1280, uint32_t _height = 720) : Title(_title), Width(_width), Height(_height){}
	};

	class CWindow {
	public:
		using EventCallbackFn = std::function<void(CEvent&)>;

		virtual ~CWindow() {}
		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void SetWidth(uint32_t _) = 0;
		virtual void SetHeight(uint32_t _) = 0;
		
		virtual void SetEventCallback(const EventCallbackFn& _callback) = 0;
		virtual void SetVSync(bool _bEnabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static CWindow* Create(const SWindowProps& _props = SWindowProps());
	};
}