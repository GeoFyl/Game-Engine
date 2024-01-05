#pragma once
#include "InputSystem.h"
#include <gainput.h>

namespace Toffee::Internal {
	class InputSystem_Gainput : public InputSystem {
	public:
		InputSystem_Gainput();

	private:

		// Inherited from Subsystem

		int Initialise() final;
		int Shutdown() final;

		// See InputSystem.h for function details

		void Update() final;
		bool IsDown(Button button) final;
		bool IsPressed(Button button) final;
		bool IsReleased(Button button) final;
		float GetMouseX() final;
		float GetMouseY() final;
		void WarpMouse(int x, int y) final;

		virtual void HandleMessage(void* msg) final;

		// Things for internal use
		enum MouseAxis { MOUSE_X = -2, MOUSE_Y = -1 };
		gainput::InputManager manager_;
		gainput::DeviceId keyboard_;
		gainput::DeviceId mouse_;
		gainput::InputMap map_ = gainput::InputMap(manager_);
	};
}
