#pragma once
#include "precompiled.h"

namespace pCore
{
	class BlenderLoader
	{
	public:

		static void Load(PandaFramework* framework, WindowFramework* window, const std::string& path, NodePath& out_model,
			bool auto_add_to_render = false)
		{
			out_model = window->load_model(framework->get_models(), path);
			out_model.set_p(90.f);

			if (auto_add_to_render)
				out_model.reparent_to(window->get_render());
			
		}
	};
}