#pragma once
#include "Application.h"
#include "ARC/Types/TybeBase.h"
#include "Log.h"
#include "ARC/Types/vector.h"

#ifdef ARC_PLATFORM_WINDOWS

extern ARC::Core::CApplication* ARC::Core::CreateApplication();

int main(int p_ArgA, char** p_ArgB)
{	
	printf("Starting-ARC----\n");

	//-------------------Log-Init-------------------//
	printf("Initializing-Logger----\n");
	bool bInitLogger = ARC::CLog::Init();
	if(!bInitLogger)
		return -1;
	else
		ARC_CORE_INFO("Logger-Initialized-Successfully");
	//-----------------Log-Init-End-----------------//
	
	//--------------Create-Application--------------//
	ARC_CORE_INFO("Creating-Application");
	ARC::Core::CApplication* App = ARC::Core::CreateApplication();
	ARC_CORE_INFO("Application:[{0}]-Created-Successfully", App->GetAppName());
	//--------------Application-Created-------------//
	
	//----------------RunLoop-Started---------------//
	ARC_CORE_INFO("Run-Loop-Started");
	App->Run();
	ARC_CORE_INFO("Run-Loop-Ended");
	//-----------------RunLoop-Ended----------------//
	
	//-------------Application-Deleting-------------//
	ARC_CORE_INFO("Destroying-Application-[]");
	delete App;
	ARC_CORE_INFO("Application-Destroyed-Successfully");
	//--------------Application-Deleted-------------//
	return 0;
}
#endif