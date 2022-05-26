#include "precompiled.h"
#include "Game.h"

namespace pCore
{
	bool Game::s_Running = true;
	CameraController* Game::s_FPCameraController = nullptr;
	PT(AsyncTaskManager) Game::s_TaskMgr = nullptr;
	PandaFramework* Game::s_PandaFramework = nullptr;
	WindowFramework* Game::s_Window = nullptr;

	Game::Game(const std::string& game_title, int width, int height, bool hide_mouse)
	{
		s_PandaFramework = new PandaFramework();
		s_PandaFramework->open_framework();

		s_TaskMgr = AsyncTaskManager::get_global_ptr();

		s_PandaFramework->set_window_title(game_title);
		s_Window = s_PandaFramework->open_window();

		WindowProperties props = s_Window->get_graphics_window()->get_properties();
		props.set_cursor_hidden(hide_mouse);
		props.set_size(width, height);
		s_Window->get_graphics_window()->request_properties(props);

		s_FPCameraController = new pCore::CameraController();
	}

	void Game::StartGame()
	{
		Thread* current_thread = Thread::get_current_thread();
		while (pCore::CameraController::s_Running)
		{
			s_PandaFramework->do_frame(current_thread);
		}

		s_PandaFramework->close_framework();
	}

	Game::~Game(){ }

}