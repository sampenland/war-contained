#pragma once

namespace pCore
{
	class Constructor
	{

	public:

		static NodePath BuildSimpleCollider(const NodePath& ground_geom_node, NodePath& attach_collider_to);
		static void MakeGeom(const std::string& name, const std::vector<LVector3> vertices);

	};
}