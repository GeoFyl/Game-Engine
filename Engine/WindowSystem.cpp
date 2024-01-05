#include "pch.h"
#include "WindowSystem.h"
#include "SystemsLocator.h"

void WindowSystem::CreateRenderer() {
	ToffeeAPI::Renderer()->CreateRenderer();
}

void WindowSystem::Resize(float width, float height) {
	ToffeeAPI::Renderer()->Resize(width, height);
}
