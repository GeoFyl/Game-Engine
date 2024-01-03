#include "pch.h"
#include "InputSystem_Gainput.h"
#include "SystemsLocator.h"

using namespace Engine::Internal;

InputSystem_Gainput* input_gainput = new InputSystem_Gainput;

InputSystem_Gainput::InputSystem_Gainput() {
	ProvideSystem(this);
	input_gainput = nullptr;
}

int InputSystem_Gainput::Initialise() {
	keyboard_ = manager_.CreateDevice<gainput::InputDeviceKeyboard>();
	mouse_ = manager_.CreateDevice<gainput::InputDeviceMouse>();

	int result;
	result = map_.MapBool(static_cast<int>(Button::NUM_0), keyboard_, gainput::Key0);
	result = map_.MapBool(static_cast<int>(Button::NUM_1), keyboard_, gainput::Key1);
	result = map_.MapBool(static_cast<int>(Button::NUM_2), keyboard_, gainput::Key2);
	result = map_.MapBool(static_cast<int>(Button::NUM_3), keyboard_, gainput::Key3);
	result = map_.MapBool(static_cast<int>(Button::NUM_4), keyboard_, gainput::Key4);
	result = map_.MapBool(static_cast<int>(Button::NUM_5), keyboard_, gainput::Key5);
	result = map_.MapBool(static_cast<int>(Button::NUM_6), keyboard_, gainput::Key6);
	result = map_.MapBool(static_cast<int>(Button::NUM_7), keyboard_, gainput::Key7);
	result = map_.MapBool(static_cast<int>(Button::NUM_8), keyboard_, gainput::Key8);
	result = map_.MapBool(static_cast<int>(Button::NUM_9), keyboard_, gainput::Key9);
	result = map_.MapBool(static_cast<int>(Button::F1), keyboard_, gainput::KeyF1);
	result = map_.MapBool(static_cast<int>(Button::F2), keyboard_, gainput::KeyF2);
	result = map_.MapBool(static_cast<int>(Button::F3), keyboard_, gainput::KeyF3);
	result = map_.MapBool(static_cast<int>(Button::F4), keyboard_, gainput::KeyF4);
	result = map_.MapBool(static_cast<int>(Button::F5), keyboard_, gainput::KeyF5);
	result = map_.MapBool(static_cast<int>(Button::F6), keyboard_, gainput::KeyF6);
	result = map_.MapBool(static_cast<int>(Button::F7), keyboard_, gainput::KeyF7);
	result = map_.MapBool(static_cast<int>(Button::F8), keyboard_, gainput::KeyF8);
	result = map_.MapBool(static_cast<int>(Button::F9), keyboard_, gainput::KeyF9);
	result = map_.MapBool(static_cast<int>(Button::F10), keyboard_, gainput::KeyF10);
	result = map_.MapBool(static_cast<int>(Button::F11), keyboard_, gainput::KeyF11);
	result = map_.MapBool(static_cast<int>(Button::F12), keyboard_, gainput::KeyF12);
	result = map_.MapBool(static_cast<int>(Button::A), keyboard_, gainput::KeyA);
	result = map_.MapBool(static_cast<int>(Button::B), keyboard_, gainput::KeyB);
	result = map_.MapBool(static_cast<int>(Button::C), keyboard_, gainput::KeyC);
	result = map_.MapBool(static_cast<int>(Button::D), keyboard_, gainput::KeyD);
	result = map_.MapBool(static_cast<int>(Button::E), keyboard_, gainput::KeyE);
	result = map_.MapBool(static_cast<int>(Button::F), keyboard_, gainput::KeyF);
	result = map_.MapBool(static_cast<int>(Button::G), keyboard_, gainput::KeyG);
	result = map_.MapBool(static_cast<int>(Button::H), keyboard_, gainput::KeyH);
	result = map_.MapBool(static_cast<int>(Button::I), keyboard_, gainput::KeyI);
	result = map_.MapBool(static_cast<int>(Button::J), keyboard_, gainput::KeyJ);
	result = map_.MapBool(static_cast<int>(Button::K), keyboard_, gainput::KeyK);
	result = map_.MapBool(static_cast<int>(Button::L), keyboard_, gainput::KeyL);
	result = map_.MapBool(static_cast<int>(Button::M), keyboard_, gainput::KeyM);
	result = map_.MapBool(static_cast<int>(Button::N), keyboard_, gainput::KeyN);
	result = map_.MapBool(static_cast<int>(Button::O), keyboard_, gainput::KeyO);
	result = map_.MapBool(static_cast<int>(Button::P), keyboard_, gainput::KeyP);
	result = map_.MapBool(static_cast<int>(Button::Q), keyboard_, gainput::KeyQ);
	result = map_.MapBool(static_cast<int>(Button::R), keyboard_, gainput::KeyR);
	result = map_.MapBool(static_cast<int>(Button::S), keyboard_, gainput::KeyS);
	result = map_.MapBool(static_cast<int>(Button::T), keyboard_, gainput::KeyT);
	result = map_.MapBool(static_cast<int>(Button::U), keyboard_, gainput::KeyU);
	result = map_.MapBool(static_cast<int>(Button::V), keyboard_, gainput::KeyV);
	result = map_.MapBool(static_cast<int>(Button::W), keyboard_, gainput::KeyW);
	result = map_.MapBool(static_cast<int>(Button::X), keyboard_, gainput::KeyX);
	result = map_.MapBool(static_cast<int>(Button::Y), keyboard_, gainput::KeyY);
	result = map_.MapBool(static_cast<int>(Button::Z), keyboard_, gainput::KeyZ);
	result = map_.MapBool(static_cast<int>(Button::ESCAPE), keyboard_, gainput::KeyEscape);
	result = map_.MapBool(static_cast<int>(Button::SPACE), keyboard_, gainput::KeySpace);
	result = map_.MapBool(static_cast<int>(Button::ENTER), keyboard_, gainput::KeyReturn);
	result = map_.MapBool(static_cast<int>(Button::TAB), keyboard_, gainput::KeyTab);
	result = map_.MapBool(static_cast<int>(Button::ALT), keyboard_, gainput::KeyAltL);
	result = map_.MapBool(static_cast<int>(Button::LCTRL), keyboard_, gainput::KeyCtrlL);
	result = map_.MapBool(static_cast<int>(Button::LSHIFT), keyboard_, gainput::KeyShiftL);
	result = map_.MapBool(static_cast<int>(Button::RCTRL), keyboard_, gainput::KeyCtrlR);
	result = map_.MapBool(static_cast<int>(Button::RSHIFT), keyboard_, gainput::KeyShiftR);
	result = map_.MapBool(static_cast<int>(Button::ARROW_UP), keyboard_, gainput::KeyUp);
	result = map_.MapBool(static_cast<int>(Button::ARROW_DOWN), keyboard_, gainput::KeyDown);
	result = map_.MapBool(static_cast<int>(Button::ARROW_LEFT), keyboard_, gainput::KeyLeft);
	result = map_.MapBool(static_cast<int>(Button::ARROW_RIGHT), keyboard_, gainput::KeyRight);
	result = map_.MapBool(static_cast<int>(Button::MOUSE_LEFT), mouse_, gainput::MouseButtonLeft);
	result = map_.MapBool(static_cast<int>(Button::MOUSE_RIGHT), mouse_, gainput::MouseButtonRight);
	result = map_.MapBool(static_cast<int>(Button::MOUSE_WHEEL), mouse_, gainput::MouseButtonMiddle);
	result = map_.MapBool(static_cast<int>(Button::MOUSE_WHEEL_UP), mouse_, gainput::MouseButtonWheelUp);
	result = map_.MapBool(static_cast<int>(Button::MOUSE_WHEEL_DOWN), mouse_, gainput::MouseButtonWheelDown);
	result = map_.MapFloat(static_cast<int>(MouseAxis::MOUSE_X), mouse_, gainput::MouseAxisX);
	result = map_.MapFloat(static_cast<int>(MouseAxis::MOUSE_Y), mouse_, gainput::MouseAxisY);

	return result;
}

int InputSystem_Gainput::Shutdown() {
	return 0;
}

void InputSystem_Gainput::Update() {
	manager_.Update();
}

bool InputSystem_Gainput::IsDown(Button button) {
	return map_.GetBool(static_cast<int>(button));
}

bool InputSystem_Gainput::IsPressed(Button button) {
	return map_.GetBoolIsNew(static_cast<int>(button));
}

bool InputSystem_Gainput::IsReleased(Button button) {
	return map_.GetBoolWasDown(static_cast<int>(button));
}

float InputSystem_Gainput::GetMouseX()
{
	
	//For whatever reason, gainput gives the coordinates as negative values, so invert return value
	return -map_.GetFloat(static_cast<int>(MouseAxis::MOUSE_X));
}

float InputSystem_Gainput::GetMouseY()
{
	//For whatever reason, gainput gives the coordinates as negative values, so invert return value
	return -map_.GetFloat(static_cast<int>(MouseAxis::MOUSE_Y));
}

void Engine::Internal::InputSystem_Gainput::WarpMouse(int x, int y) {
#ifdef _WIN32||_WIN64
	tagPOINT screen_cursor_pos;
	GetCursorPos(&screen_cursor_pos);
	SetCursorPos(x + screen_cursor_pos.x - GetMouseX(), y + screen_cursor_pos.y - GetMouseY());
#endif
}

void InputSystem_Gainput::HandleMessage(void* msg) {
	manager_.HandleMessage(*reinterpret_cast<MSG*>(msg));
}

