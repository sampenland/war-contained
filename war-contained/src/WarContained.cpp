#include "precompiled.h"
#include "core/CameraController.h"
#include "core/Game.h"

#define WC_VERSION "0.0.1"

int main(int argc, char* argv[]) 
{
	pCore::Game* game = new pCore::Game("War :: Contained v." + std::string(WC_VERSION), 1280, 720, true);

	NodePath mdl_level = pCore::Game::s_Window->load_model(pCore::Game::s_PandaFramework->get_models(), "res/models/level1/level1map.gltf");
	mdl_level.reparent_to(pCore::Game::s_Window->get_render());
	mdl_level.set_z(-2);

	PT(AmbientLight) sun = new AmbientLight("Sun");
	NodePath sun_np = pCore::Game::s_Window->get_render().attach_new_node(sun);
	sun_np.set_color(LColor(254 / 255, 1, 242 / 255, 0.5f));
	pCore::Game::s_Window->get_render().set_light(sun_np);

	game->StartGame();

	delete game;

	

	/*
			# create a collision mesh for each of the loaded models

			model_node = model.node()
			collision_node = CollisionNode(model_node.name)
			collision_mesh = collision_root.attach_new_node(collision_node)
			# collision nodes are hidden by default
			collision_mesh.show()

			for geom in model_node.modify_geoms() :

				geom.decompose_in_place()
				vertex_data = geom.modify_vertex_data()
				vertex_data.format = GeomVertexFormat.get_v3()
				view = memoryview(vertex_data.arrays[0]).cast("B").cast("f")
				index_list = geom.primitives[0].get_vertex_list()
				index_count = len(index_list)

				for indices in(index_list[i:i + 3] for i in range(0, index_count, 3)) :
					points = [Point3(*view[index * 3:index * 3 + 3]) for index in indices]
					coll_poly = CollisionPolygon(*points)
					collision_node.add_solid(coll_poly)

	*/

	// ------------------------------------------------
	// Create collision shape from geometry of model
	// ------------------------------------------------

	//NodePath ground_geom = mdl_level.find("**/Ground");
	/*PT(GeomNode) geomNode = DCAST(GeomNode, ground_geom.node());
	CPT(Geom) geom = geomNode->get_geom(0);
	CPT(GeomVertexData) vdata = geom->get_vertex_data();

	GeomVertexReader vertex(vdata, "vertex");
	while (!vertex.is_at_end()) 
	{
		LVector3 v = vertex.get_data3();

	}*/
	
	// -----------------------------------------------
	
	return 0;

}