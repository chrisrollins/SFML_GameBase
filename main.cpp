#include "Screen.h"
#include "Menu.h"
#include "DebugManager.h"
#include <string>

#ifdef _MSC_VER
#include "Windows.h"
#endif

using namespace Engine;

int main(int argc, char** argv)
{
	#ifndef _DEBUG
	#ifdef _MSC_VER
	FreeConsole();
	#endif
	#endif

	for (int i = 0; i < argc; i++)
	{
		string arg(argv[i]);
	#ifdef _DEBUG
		if (arg == "RESOURCE_REPORTING") { DebugManager::EnableMessageType(DebugManager::MessageType::RESOURCE_REPORTING); }
		else if (arg == "PERFORMANCE_REPORTING") { DebugManager::EnableMessageType(DebugManager::MessageType::PERFORMANCE_REPORTING); }
		else if (arg == "ERROR_REPORTING") { DebugManager::EnableMessageType(DebugManager::MessageType::ERROR_REPORTING); }
	#endif
	}

	// these are static members that should be set before rendering a screen. 
	// the window will be locked at these values after rendering has started.
	Screen::windowWidth = 1024;
	Screen::windowHeight = 768;
	Screen::windowTitle = "Cursed Zombie";

	Menu* menu = new Menu(true);
	menu->start();

	return 0;
}