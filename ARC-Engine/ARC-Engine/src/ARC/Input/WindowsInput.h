#pragma once
#include "ARC\Input\Input.h"
namespace ARC{
	class ARC_API CWindowsInput : public CInput
	{
	protected:
		virtual bool IsKeyPressedImpl(int _keycode) const override;
		virtual bool IsMouseButtonPressedImpl(int _button) const override;
		virtual float GetMouseXImpl() const override;
		virtual float GetMouseYImpl() const override;

		TVec2<> GetMouseXYImpl() const override;

		virtual int GetKeyImpl(int _keycode) override;

	};
}