#pragma once

#include <unordered_map>

namespace pCore
{
	enum class Keys
	{
		Forward,
		Backward,
		Left,
		Right,
		Escape // TODO: abstract to main loop /engine
	};

	class CameraController
	{

	public:

		CameraController();
		~CameraController();

		static AsyncTask::DoneStatus Update(GenericAsyncTask* task, void* data);

		static inline void SetMouseSensitivity(float sens) { s_MouseSensitivity = sens; }
		static inline float GetMouseSensitivity() { return s_MouseSensitivity; }

		static inline void SetMoveSpeed(float speed) { s_MoveSpeed = speed; }
		static inline float GetMoveSpeed() { return s_MoveSpeed; }

		static void KeyPress(const Event*, void*);
		static void KeyUp(const Event*, void*);

		static bool s_Running;

	protected:

		static void HandleKeyboard();
		static void HandleMouse();

		static NodePath s_Camera;

	private:

		AsyncTaskManager* m_TaskMgr = nullptr;
		GenericAsyncTask* m_UpdateTask = nullptr;

		static float s_MouseSensitivity;
		static float s_MoveSpeed;
		static float s_CenterX;
		static float s_CenterY;
		static bool s_UnlockMouse;

		static std::unordered_map<Keys, bool> s_ControlKeys;

	};
}

