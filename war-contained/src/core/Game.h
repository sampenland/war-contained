#pragma once
#include "core/CameraController.h"

namespace pCore
{
	class Game
	{

	public:

		void StartGame();

		Game(const std::string& game_title, int width, int height, bool hide_mouse = false);
		~Game();


	public:

		static bool s_Running;

		static CameraController* s_FPCameraController;
		static PT(AsyncTaskManager) s_TaskMgr;
		static PandaFramework* s_PandaFramework;
		static WindowFramework* s_Window;

	private:


	};
}