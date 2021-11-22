#pragma once
#include <string>
#include "ARC\Application.h"
#include "ARC\Layers\Layer.h"

namespace ARC { class CEvent; }

class CExampleLayer : public ARC::CLayer
{
public:
	CExampleLayer() : ARC::CLayer("Example"){}
	void OnUpdate() override;
	void OnEvent(ARC::CEvent& _event) override;

	void OnGuiRender() override;
};

class CSandbox : public ARC::Core::CApplication
{
public:
	CSandbox();
	~CSandbox() {};

	inline virtual std::string GetAppName() override { return "Sandbox"; };
};
