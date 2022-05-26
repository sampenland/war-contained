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
		std::vector<LVector3> normals;
		GeomVertexReader vertex(vdata, "vertex");
		GeomVertexReader normal(vdata, "normal");
		while (!vertex.is_at_end())
		{
			LVector3 p = vertex.get_data3();
			LVector3 n = normal.get_data3();
			std::cout << p << std::endl;
			std::cout << n << std::endl;
			vertices.push_back(p);
			normals.push_back(n);
		}

		int vertices_count = (int)vertices.size();
		for (int i = 0; i < (int)vertices.size(); i+=4)
		{
			LVecBase3 a = vertices[i];
			LVecBase3 b = vertices[i + 1];
			LVecBase3 c = vertices[i + 2];
			LVecBase3 d = vertices[i + 3];

			PT(CollisionPolygon) poly = new CollisionPolygon(a, b, c, d);
			collision_node->add_solid(poly);
		}

		// -----------------------------------------------

		return collision_root;
	}
}