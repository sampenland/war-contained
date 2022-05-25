#pragma once

#include <unordered_map>

namespace pCore
{
	enum class CamConDirs
	{
		Forward,
		Backward,
		Left,
		Right
	};

	class CameraController
	{

	public:

		CameraController(PandaFramework* framework, WindowFramework* window);
		~CameraController();

		static AsyncTask::DoneStatus Update(GenericAsyncTask* task, void* data);

		static inline void SetMouseSensitivity(float sens) { s_MouseSensitivity = sens; }
		static inline float GetMouseSensitivity() { return s_MouseSensitivity; }

		static inline void SetMoveSpeed(float speed) { s_MoveSpeed = speed; }
		static inline float GetMoveSpeed() { return s_MoveSpeed; }

		static void KeyPress(const Event*, void*);
		static void KeyUp(const Event*, void*);

	protected:

		static void HandleKeyboard();
		static void HandleMouse();

		static NodePath s_Camera;

	private:

		AsyncTaskManager* m_TaskMgr = nullptr;
		GenericAsyncTask* m_UpdateTask = nullptr;

		static float s_MouseSensitivity;
		static float s_MoveSpeed;

		static std::unordered_map<CamConDirs, bool> s_ControlKeys;

	};
}

