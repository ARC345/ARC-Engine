#pragma once
#include "Application.h"

#ifdef ARC_PLATFORM_WINDOWS

extern ARC::Core::CApplication* ARC::Core::CreateApplication();

int main(int p_ArgA, char** p_ArgB)
{
	printf("----Starting ARC----\n");
	printf("----Creating Application----\n");
	ARC::Core::CApplication* App = ARC::Core::CreateApplication();
	printf("----Application Created Successfully----\n");
	printf("----Run Loop Started----\n");
	App->Run();
	printf("----Run Loop Ended----n");
	printf("----Deleting Application----\n\n");
	delete App;
	printf("----Application Destroyed Successfully----\n");
	return 0;
}
#endif