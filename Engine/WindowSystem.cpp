#include "pch.h"
#include "WindowSystem.h"
#include "SystemsLocator.h"

void WindowSystem::CreateRenderer() {
	SystemsAPI::Renderer()->CreateRenderer();
}

void WindowSystem::Resize(float width, float height) {
	SystemsAPI::Renderer()->Resize(width, height);
}
