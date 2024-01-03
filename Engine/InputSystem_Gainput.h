#pragma once
#include "InputSystem.h"
#include <gainput.h>

namespace Engine::Internal {
	class InputSystem_Gainput : public InputSystem {
	public:
		InputSystem_Gainput();
		int Initialise() final;
		int Shutdown() final;

		void Update();
		bool IsDown(Button button);
		bool IsPressed(Button button);
		bool IsReleased(Button button);
		float GetMouseX();
		float GetMouseY();
		void WarpMouse(int x, int y);

	protected:
		enum MouseAxis { MOUSE_X = -2, MOUSE_Y = -1 };

		virtual void HandleMessage(void* msg);

		gainput::InputManager manager_;
		gainput::DeviceId keyboard_;
		gainput::DeviceId mouse_;
		gainput::InputMap map_ = gainput::InputMap(manager_);
	};
}
