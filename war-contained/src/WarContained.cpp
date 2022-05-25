#include "precompiled.h"
#include "core/CameraController.h"

#define WC_VERSION "0.0.1"

int main(int argc, char* argv[]) 
{
	PandaFramework framework;
	framework.open_framework(argc, argv);
	PT(AsyncTaskManager) task_mgr = AsyncTaskManager::get_global_ptr();

	framework.set_window_title("War :: Contained v" + std::string(WC_VERSION));
	WindowFramework* window = framework.open_window();

	NodePath mdl_level;
	pCore::BlenderLoader::Load(&framework, window, "res/models/level1/level1.gltf", mdl_level, true);
	mdl_level.set_z(-2);
	
	pCore::CameraController* cam_controller = new pCore::CameraController(&framework, window);

	PT(AmbientLight) sun = new AmbientLight("Sun");
	NodePath sun_np = window->get_render().attach_new_node(sun);
	sun_np.set_color(LColor(254 / 255, 1, 242 / 255, 0.5f));
	window->get_render().set_light(sun_np);	

	framework.main_loop();
	framework.close_framework();

	delete cam_controller;

	return (0);
}