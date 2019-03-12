#include "Screen.h"
#include "Menu.h"
#include "ScoreBoard.h"

using namespace Engine;

int main(int argc, char** argv)
{
	//these are static members that should be set before rendering a screen. the window will be locked at these values after rendering has started.
	Screen::windowWidth = 1024;
	Screen::windowHeight = 768;
	Screen::windowTitle = "Game!";
	
	Menu* menu = new Menu();
	menu->start();

	return 0;
}