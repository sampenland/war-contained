#include "precompiled.h"

#define WC_VERSION "0.0.1"

int main(int argc, char* argv[]) 
{
	// Open a new window framework
	PandaFramework framework;
	framework.open_framework(argc, argv);

	// Set the window title and open the window
	framework.set_window_title("War :: Contained v" + std::string(WC_VERSION));
	WindowFramework* window = framework.open_window();

	NodePath mdl_level = window->load_model(framework.get_models(), "res/models/level1/level1.glb");
	
	// Here is room for your own code

	// Do the main loop, equal to run() in python
	framework.main_loop();
	framework.close_framework();
	return (0);
}