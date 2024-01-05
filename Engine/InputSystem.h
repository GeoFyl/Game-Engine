#pragma once
#include "pch.h"
#include "Subsystem.h"

namespace Toffee {
	// A keyboard or mouse button
	enum class Button {
		NUM_0,
		NUM_1,
		NUM_2,
		NUM_3,
		NUM_4,
		NUM_5,
		NUM_6,
		NUM_7,
		NUM_8,
		NUM_9,
		
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		ESCAPE,
		SPACE,
		ENTER,
		TAB,
		ALT,
		LCTRL,
		LSHIFT,
		RCTRL,
		RSHIFT,

		ARROW_UP,
		ARROW_DOWN,
		ARROW_LEFT,
		ARROW_RIGHT,

		MOUSE_RIGHT,
		MOUSE_LEFT,
		MOUSE_WHEEL,
		MOUSE_WHEEL_UP,
		MOUSE_WHEEL_DOWN
	};
}

namespace Toffee::Internal {
	/// <summary>
	/// Interface base class to input system.
	/// </summary>
	class InputSystem : public Subsystem {
	public:
		InputSystem() { type_ = SubsytemType::INPUT; }

		// Returns whether a button is currently down.
		virtual bool IsDown(Button button) = 0;
		// Returns whether a button has just been pressed.
		virtual bool IsPressed(Button button) = 0;
		// Returns whether a button has just been released.
		virtual bool IsReleased(Button button) = 0;
		// Get the mouse x coordinate relative to the window.
		// (0,0) is the top left of the window.
		virtual float GetMouseX() = 0;
		// Get the mouse y coordinate relative to the window.
		// (0,0) is the top left of the window.
		virtual float GetMouseY() = 0;
		// Set position of the mouse pointer.
		virtual void WarpMouse(int x, int y) = 0;

	protected:
		friend class Core;
		friend class EventSystem;
		virtual void Update() = 0;
		// Recieve and process an input message.
		virtual void HandleMessage(void* msg) = 0;
	};
}
