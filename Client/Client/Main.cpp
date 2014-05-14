#include <iostream>
#include "..\..\Common\GeneralMath.h"
#include "Application.h"
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int argc, char* argv[])
{
	Application app;
	app.run();
	return 0;
}