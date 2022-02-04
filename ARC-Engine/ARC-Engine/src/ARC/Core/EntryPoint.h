#pragma once
#include "Application.h"
#include "ARC/Types/TybeBase.h"
#include "Log.h"
#include "ARC/Types/vector.h"
#include "ARC/Profiling/Timer.h"

#ifdef ARC_PLATFORM_WINDOWS

extern ARC::Core::CApplication* ARC::Core::CreateApplication();

int main(int p_ArgA, char** p_ArgB)
{	
	printf("Starting-ARC----\n");

	//-------------------Log-Init-------------------//
	ARC_PROFILE_BEGIN_SESSION("CSandBox2D::Log::Init", "CSandBox2D-Log-Init.json");
	printf("Initializing-Logger----\n");
	bool bInitLogger = ARC::CLog::Init();
	if(!bInitLogger)
		return -1;
	else
		ARC_CORE_INFO("Logger-Initialized-Successfully");
	ARC_PROFILE_END_SESSION();
	//-----------------Log-Init-End-----------------//
	
	//--------------Create-Application--------------//
	ARC_PROFILE_BEGIN_SESSION("CSandBox2D::Create::Application", "CSandBox2D-Create-Application.json");
	ARC_CORE_INFO("Creating-Application");
	ARC::Core::CApplication* App = ARC::Core::CreateApplication();
	ARC_CORE_INFO("Application:[{0}]-Created-Successfully", App->GetAppName());
	ARC_PROFILE_END_SESSION();
	//--------------Application-Created-------------//
	
	//----------------RunLoop-Started---------------//
	ARC_PROFILE_BEGIN_SESSION("CSandBox2D::RunLoop", "CSandBox2D-RunLoop.json");
	ARC_CORE_INFO("Run-Loop-Started");
	App->Run();
	ARC_CORE_INFO("Run-Loop-Ended");
	ARC_PROFILE_END_SESSION();
	//-----------------RunLoop-Ended----------------//
	
	//-------------Application-Deleting-------------//
	ARC_PROFILE_BEGIN_SESSION("CSandBox2D::End", "CSandBox2D-End.json");
	ARC_CORE_INFO("Destroying-Application-[]");
	delete App;
	ARC_CORE_INFO("Application-Destroyed-Successfully");
	ARC_PROFILE_END_SESSION();
	//--------------Application-Deleted-------------//
	return 0;
}
#endif