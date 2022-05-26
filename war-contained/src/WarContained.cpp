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

	NodePath player = pCore::Game::s_Window->load_model(pCore::Game::s_PandaFramework->get_models(),
		"res/models/level1/player.gltf");
	player.reparent_to(pCore::Game::s_Window->get_render());
	player.set_collide_mask(CollideMask::all_off());

	CollisionRay ray = CollisionRay(player.get_pos(), LVector3(0, 0, -1));
	CollisionNode* ray_node = new CollisionNode("ray");
	NodePath ray_np = player.attach_new_node(ray_node);
	ray_node->add_solid(&ray);
	ray_np.show();
	ray_np.ls();
	
	PT(CollisionHandlerFloor) ground = new CollisionHandlerFloor;
	ground->set_max_velocity(2);	

	CollisionTraverser traverser;
	traverser.set_respect_prev_transform(true);
	traverser.traverse(pCore::Game::s_Window->get_render());
	traverser.add_collider(ray_np, ground);
	ground->add_collider(ray_np, player);

	traverser.show_collisions(pCore::Game::s_Window->get_render());

	player.set_y(6);

	game->StartGame();

	delete game;
	
	return 0;

}