#pragma once
#include <string>
#include "ARC\Application.h"

namespace ARC { class CEvent; }

class ExampleLayer : public ARC::Layer
{
public:
	ExampleLayer() : ARC::Layer("Example"){}
	void OnUpdate() override;
	void OnEvent(ARC::CEvent& _event) override;
};

class CSandbox : public ARC::Core::CApplication
{
public:
	CSandbox();
	~CSandbox() {};

	inline virtual std::string GetAppName() override { return "Sandbox"; };
};
