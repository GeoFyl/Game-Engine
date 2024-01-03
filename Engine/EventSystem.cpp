#include "pch.h"
#include "EventSystem.h"
#include "SystemsLocator.h"

void EventSystem::HandleInputEvent(void* msg) {
	SystemsAPI::Input()->HandleMessage(msg);
}

void EventSystem::HandleWindowEvent(WindowEvent& e) {
	SystemsAPI::Window()->HandleWindowEvent(e);
}
