#include "precompiled.h"
#include "core/CameraController.h"

#define WC_VERSION "0.0.1"

int main(int argc, char* argv[]) 
{
	// Open a new window framework
	PandaFramework framework;
	framework.open_framework(argc, argv);
	PT(AsyncTaskManager) task_mgr = AsyncTaskManager::get_global_ptr();

	// Set the window title and open the window
	framework.set_window_title("War :: Contained v" + std::string(WC_VERSION));
	WindowFramework* window = framework.open_window();

	NodePath mdl_level = window->load_model(framework.get_models(), "res/models/level1/level1.gltf");
	mdl_level.reparent_to(window->get_render());
	mdl_level.set_p(90.f);
	mdl_level.set_z(-2);
	
	pCore::CameraController* cam_controller = new pCore::CameraController(&framework, window);

	framework.main_loop();
	framework.close_framework();

	delete cam_controller;

	return (0);
}