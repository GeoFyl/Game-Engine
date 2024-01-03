#pragma once
#include "pch.h"
#include "Subsystem.h"

namespace Engine {
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

namespace Engine::Internal {


	/// <summary>
	/// Interface base class to input system
	/// </summary>
	class InputSystem : public Subsystem {
	public:
		InputSystem() { type_ = SubsytemType::INPUT; }
		virtual void Update() = 0;
		virtual bool IsDown(Button button) = 0;
		virtual bool IsPressed(Button button) = 0;
		virtual bool IsReleased(Button button) = 0;
		virtual float GetMouseX() = 0;
		virtual float GetMouseY() = 0;
		virtual void WarpMouse(int x, int y) = 0;
	protected:
		friend class EventSystem;
		virtual void HandleMessage(void* msg) = 0;
	};
}
