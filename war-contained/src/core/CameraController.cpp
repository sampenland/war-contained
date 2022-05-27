#include "precompiled.h"
#include "CameraController.h"
#include "Game.h"

namespace pCore
{
	NodePath CameraController::s_Camera;
	std::unordered_map<Keys, bool> CameraController::s_ControlKeys;
	float CameraController::s_MouseSensitivity = 0.15f;
	float CameraController::s_MoveSpeed = 10.f;
	float CameraController::s_CenterX = 0.f;
	float CameraController::s_CenterY = 0.f;
	bool CameraController::s_UnlockMouse = false;

	CollisionRay* CameraController::s_Ray = nullptr;
	CollisionNode* CameraController::s_RayNode = nullptr;
	CollisionSphere* CameraController::s_PlayerCollisonSolid = nullptr;
	CollisionNode* CameraController::s_PlayerCollider = nullptr;
	NodePath CameraController::s_Player;
	NodePath CameraController::s_CamToGround;

	// TODO: abstract this into an engine loop class
	bool CameraController::s_Running = true;
	// ---------------------------------------------
	
	CameraController::CameraController()
	{
		Game::s_Window->enable_keyboard();

		Game::s_PandaFramework->define_key("escape-up", "End Game", &pCore::CameraController::KeyUp, (void*)pCore::Keys::Escape);

		Game::s_PandaFramework->define_key("w", "Move camera Forward", &pCore::CameraController::KeyPress, (void*)pCore::Keys::Forward);
		Game::s_PandaFramework->define_key("s", "Move camera Backward", &pCore::CameraController::KeyPress, (void*)pCore::Keys::Backward);
		Game::s_PandaFramework->define_key("a", "Move camera Left", &pCore::CameraController::KeyPress, (void*)pCore::Keys::Left);
		Game::s_PandaFramework->define_key("d", "Move camera Right", &pCore::CameraController::KeyPress, (void*)pCore::Keys::Right);

		Game::s_PandaFramework->define_key("w-up", "Move camera Forward", &pCore::CameraController::KeyUp, (void*)pCore::Keys::Forward);
		Game::s_PandaFramework->define_key("s-up", "Move camera Backward", &pCore::CameraController::KeyUp, (void*)pCore::Keys::Backward);
		Game::s_PandaFramework->define_key("a-up", "Move camera Left", &pCore::CameraController::KeyUp, (void*)pCore::Keys::Left);
		Game::s_PandaFramework->define_key("d-up", "Move camera Right", &pCore::CameraController::KeyUp, (void*)pCore::Keys::Right);

		m_TaskMgr = AsyncTaskManager::get_global_ptr();
		m_UpdateTask = new GenericAsyncTask("CameraControllerUpdate", &CameraController::Update, nullptr);
		m_TaskMgr->add(m_UpdateTask);

		s_Camera = Game::s_Window->get_camera_group();

		s_CenterX = Game::s_Window->get_graphics_window()->get_x_size() / 2; // Horizontal center of screen.
		s_CenterY = Game::s_Window->get_graphics_window()->get_y_size() / 2; // Vertical center of screen.

		Game::s_Window->get_camera(0)->get_lens()->set_fov(90);
		Game::s_Window->get_camera(0)->get_lens()->set_near(0.1); // Prevents near clipping with objects.

		s_Ray = new CollisionRay(s_Camera.get_pos(), LVector3(0, 0, -1));
		s_RayNode = new CollisionNode("cam_to_ground_ray");
		s_CamToGround = s_Camera.attach_new_node(s_RayNode);
		s_RayNode->add_solid(s_Ray);
		s_CamToGround.show();

		PT(CollisionHandlerFloor) ground = new CollisionHandlerFloor();
		ground->set_max_velocity(2);

		CollisionTraverser traverser = CollisionTraverser("traverser");
		traverser.set_respect_prev_transform(true);
		traverser.traverse(pCore::Game::s_Window->get_render());
		
		s_PlayerCollisonSolid = new CollisionSphere(s_Camera.get_pos(), 1.7f);
		s_PlayerCollider = new CollisionNode("player");
		s_PlayerCollider->set_collide_mask(CollideMask::all_off());
		s_Player = s_Camera.attach_new_node(s_PlayerCollider);
		s_PlayerCollider->add_solid(s_PlayerCollisonSolid);
		s_Player.reparent_to(Game::s_Window->get_render());

		traverser.add_collider(s_CamToGround, ground);
		ground->add_collider(s_CamToGround, s_Player);

		traverser.show_collisions(pCore::Game::s_Window->get_render());
		
	}

	CameraController::~CameraController()
	{
		m_UpdateTask->remove();
		
		if (m_UpdateTask)
			delete m_UpdateTask;
	}

	void CameraController::KeyPress(const Event* e, void* data)
	{
		Keys key = (Keys)(int)data;
		s_ControlKeys[key] = true;
	}

	void CameraController::KeyUp(const Event* e, void* data)
	{
		Keys key = (Keys)(int)data;
		s_ControlKeys[key] = false;

		// TODO: abstract this to engine/main loop
		if (key == Keys::Escape)
		{
			s_Running = false;
		}
		// ---------------------------------------
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
		if (s_ControlKeys[Keys::Forward])
		{
			float y = s_Camera.get_y();
			s_Camera.set_y(s_Camera, s_MoveSpeed * dt);
		}

		if (s_ControlKeys[Keys::Backward])
		{
			float y = s_Camera.get_y();
			s_Camera.set_y(s_Camera, -s_MoveSpeed * dt);
		}

		if (s_ControlKeys[Keys::Right])
		{
			float x = s_Camera.get_x();
			s_Camera.set_x(s_Camera, s_MoveSpeed * 0.75f * dt);
		}

		if (s_ControlKeys[Keys::Left])
		{
			float x = s_Camera.get_x();
			s_Camera.set_x(s_Camera, -s_MoveSpeed * 0.75f * dt);
		}
	}

	void CameraController::HandleMouse()
	{
		if (Game::s_Window->get_graphics_window() && !s_UnlockMouse) {

			/* Gets the difference between the mouse's horiz position and the horiz center of the screen  */
			int mx = Game::s_Window->get_graphics_window()->get_pointer(0).get_x() - s_CenterX;

			/* Difference between the mouse's vert position and the vert center of the screen  */
			int my = Game::s_Window->get_graphics_window()->get_pointer(0).get_y() - s_CenterY;

			/* p is the camera's pitch, h is the camera's heading (A.K.A. Yaw) */
			float p = s_Camera.get_p(), h = s_Camera.get_h();

			s_Camera.set_h(h - mx * s_MouseSensitivity);
			s_Camera.set_p(p - my * s_MouseSensitivity);

			// Keeps mouse locked in window.
			Game::s_Window->get_graphics_window()->move_pointer(0, s_CenterX, s_CenterY);

			// Locks the camera's pitch, to prevent the player from looking upside down.
			if (p > 90)
				s_Camera.set_p(90); 
			else if (p < -90)
				s_Camera.set_p(-90); 

		}
	}
}
