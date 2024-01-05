#include "pch.h"
#include "EventSystem.h"
#include "SystemsLocator.h"

void EventSystem::HandleInputEvent(void* msg) {
	ToffeeAPI::Input()->HandleMessage(msg);
}

void EventSystem::HandleWindowEvent(WindowEvent& e) {
	ToffeeAPI::Window()->HandleWindowEvent(e);
}
