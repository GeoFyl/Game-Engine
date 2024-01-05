#include "pch.h"
#include "WorldSystem.h"
#include "SystemsLocator.h"

void Toffee::Internal::WorldSystem::SwitchPhysicsScene(std::string scene) {
	ToffeeAPI::Physics()->SwitchScene(scene);
}
