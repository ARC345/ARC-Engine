#pragma once
#include "Application.h"
#include "ARC/Types/TybeBase.h"
#include "Log.h"
#include "ARC/Types/vector.h"
#include "Macros.h"
#include "ARC/Profiling/Timer.h"

#ifdef ARC_PLATFORM_WINDOWS

extern arc::application* arc::create_application();

int main(int arg_a, char** arg_b)
{	
	printf("Starting-ARC----\n");
	
	//-------------------Log-init-------------------//
	ARC_PROFILE_BEGIN_SESSION("CSandBox2D::Log::init", "logs/CSandBox2D-Log-init.json");
	printf("Initializing-Logger----\n");
	bool bInitLogger = arc::CLog::init();
	if(!bInitLogger)
		return -1;
	else
		ARC_CORE_INFO("Logger-Initialized-Successfully");
	ARC_PROFILE_END_SESSION();
	//-----------------Log-init-end-----------------//
	
	//--------------create-Application--------------//
	ARC_PROFILE_BEGIN_SESSION("CSandBox2D::create::Application", "logs/CSandBox2D-create-Application.json");
	ARC_CORE_INFO("Creating-Application");
	arc::application* App = arc::create_application();
	ARC_CORE_INFO("Application:[{0}]-Created-Successfully", App->get_app_name());
	ARC_PROFILE_END_SESSION();
	//--------------Application-Created-------------//
	
	//----------------RunLoop-Started---------------//
	ARC_PROFILE_BEGIN_SESSION("CSandBox2D::RunLoop", "logs/CSandBox2D-RunLoop.json");
	ARC_CORE_INFO("run-Loop-Started");
	App->run();
	ARC_CORE_INFO("run-Loop-Ended");
	ARC_PROFILE_END_SESSION();
	//-----------------RunLoop-Ended----------------//
	
	//-------------Application-Deleting-------------//
	ARC_PROFILE_BEGIN_SESSION("CSandBox2D::end", "logs/CSandBox2D-end.json");
	ARC_CORE_INFO("Destroying-Application-[]");
	delete App;
	ARC_CORE_INFO("Application-Destroyed-Successfully");
	ARC_PROFILE_END_SESSION();
	//--------------Application-Deleted-------------//
	return 0;
}
#endif