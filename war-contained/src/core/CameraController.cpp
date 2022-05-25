#include "precompiled.h"
#include "CameraController.h"

namespace pCore
{
	NodePath CameraController::s_Camera;
	std::unordered_map<CamConDirs, bool> CameraController::s_ControlKeys;
	float CameraController::s_MouseSensitivity = 1.f;
	float CameraController::s_MoveSpeed = 10.f;

	CameraController::CameraController(PandaFramework* framework, WindowFramework* window)
	{
		window->enable_keyboard();

		framework->define_key("w", "Move camera Forward", &pCore::CameraController::KeyPress, (void*)pCore::CamConDirs::Forward);
		framework->define_key("s", "Move camera Backward", &pCore::CameraController::KeyPress, (void*)pCore::CamConDirs::Backward);
		framework->define_key("a", "Move camera Left", &pCore::CameraController::KeyPress, (void*)pCore::CamConDirs::Left);
		framework->define_key("d", "Move camera Right", &pCore::CameraController::KeyPress, (void*)pCore::CamConDirs::Right);

		framework->define_key("w-up", "Move camera Forward", &pCore::CameraController::KeyUp, (void*)pCore::CamConDirs::Forward);
		framework->define_key("s-up", "Move camera Backward", &pCore::CameraController::KeyUp, (void*)pCore::CamConDirs::Backward);
		framework->define_key("a-up", "Move camera Left", &pCore::CameraController::KeyUp, (void*)pCore::CamConDirs::Left);
		framework->define_key("d-up", "Move camera Right", &pCore::CameraController::KeyUp, (void*)pCore::CamConDirs::Right);

		m_TaskMgr = AsyncTaskManager::get_global_ptr();
		m_UpdateTask = new GenericAsyncTask("CameraControllerUpdate", &CameraController::Update, nullptr);
		m_TaskMgr->add(m_UpdateTask);

		s_Camera = window->get_camera_group();
	}

	CameraController::~CameraController()
	{
		m_UpdateTask->remove();
		
		if (m_UpdateTask)
			delete m_UpdateTask;
	}

	void CameraController::KeyPress(const Event* e, void* data)
	{
		CamConDirs direction = (CamConDirs)(int)data;
		s_ControlKeys[direction] = true;
	}

	void CameraController::KeyUp(const Event* e, void* data)
	{
		CamConDirs direction = (CamConDirs)(int)data;
		s_ControlKeys[direction] = false;
	}

	AsyncTask::DoneStatus CameraController::Update(GenericAsyncTask* task, void* data)
	{

		CameraController::HandleKeyboard();
		CameraController::HandleMouse();

		return AsyncTask::DS_cont;
	}

	void CameraController::HandleKeyboard()
	{
		float dt = ClockObject::get_global_clock()->get_dt();
		if (s_ControlKeys[CamConDirs::Forward])
		{
			float y = s_Camera.get_y();
			s_Camera.set_y(y + (s_MoveSpeed * dt));
		}

		if (s_ControlKeys[CamConDirs::Backward])
		{
			float y = s_Camera.get_y();
			s_Camera.set_y(y - (s_MoveSpeed * dt));
		}

		if (s_ControlKeys[CamConDirs::Right])
		{
			float x = s_Camera.get_x();
			s_Camera.set_x(x + (s_MoveSpeed * 0.75f * dt));
		}

		if (s_ControlKeys[CamConDirs::Left])
		{
			float x = s_Camera.get_x();
			s_Camera.set_x(x - (s_MoveSpeed * 0.75f * dt));
		}
	}

	void CameraController::HandleMouse()
	{

	}
}
