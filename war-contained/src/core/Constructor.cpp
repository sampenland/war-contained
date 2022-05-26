#include "precompiled.h"
#include "Constructor.h"

namespace pCore
{
	NodePath Constructor::BuildSimpleCollider(const NodePath& ground_geom_node, NodePath& attach_collider_to)
	{
		// ------------------------------------------------
		// Create collision shape from geometry of model
		// ------------------------------------------------
		NodePath collision_root = NodePath(ground_geom_node.get_name() + "_collision_root");

		collision_root.reparent_to(attach_collider_to);
		CollisionNode* collision_node = new CollisionNode(ground_geom_node.get_name() + "_collider");
		NodePath collision_mesh = collision_root.attach_new_node(collision_node);
		collision_mesh.show();

		PT(GeomNode) geomNode = DCAST(GeomNode, ground_geom_node.node());
		CPT(Geom) geom = geomNode->get_geom(0);
		CPT(GeomVertexData) vdata = geom->get_vertex_data();
		
		std::vector<LVector3> vertices;
		GeomVertexReader vertex(vdata, "vertex");
		while (!vertex.is_at_end())
		{
			LVector3 p = vertex.get_data3();
			vertices.push_back(p);
		}

		for (int i = 0; i < (int)vertices.size(); i+=3)
		{
			LVecBase3 a = vertices[i];
			LVecBase3 b = vertices[i + 1];
			LVecBase3 c = vertices[i + 2];

			PT(CollisionPolygon) poly = new CollisionPolygon(a, b, c);
			collision_node->add_solid(poly);
		}

		// -----------------------------------------------

		return collision_root;
	}

	void Constructor::MakeGeom(const std::string& name, const std::vector<LVector3> vertices)
	{
		// Setup 
		PT(GeomVertexData) vdata;
		vdata = new GeomVertexData(name, GeomVertexFormat::get_v3(), Geom::UH_static);
		vdata->set_num_rows(1);

		GeomVertexWriter vertex(vdata, "vertex");

		for (auto v : vertices)
		{
			vertex.add_data3(v);
		}
	}
}