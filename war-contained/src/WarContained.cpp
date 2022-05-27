#include "precompiled.h"
#include "core/CameraController.h"
#include "core/Game.h"
#include "core/Constructor.h"

#define WC_VERSION "0.0.1"

int main(int argc, char* argv[]) 
{
	pCore::Game* game = new pCore::Game("War :: Contained v." + std::string(WC_VERSION), 1280, 720, true);

	NodePath mdl_level = pCore::Game::s_Window->load_model(pCore::Game::s_PandaFramework->get_models(), 
		"res/models/level1/level1map.gltf");
	mdl_level.reparent_to(pCore::Game::s_Window->get_render());

	PT(AmbientLight) sun = new AmbientLight("Sun");
	NodePath sun_np = pCore::Game::s_Window->get_render().attach_new_node(sun);
	sun_np.set_color(LColor(254 / 255, 1, 242 / 255, 0.5f));
	pCore::Game::s_Window->get_render().set_light(sun_np);
		
	mdl_level.set_collide_mask(CollideMask::all_on());

	pCore::Game::s_FPCameraController->SetZ(10);

	game->StartGame();

	delete game;
	
	return 0;

}