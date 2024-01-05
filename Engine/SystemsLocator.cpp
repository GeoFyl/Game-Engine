#include "pch.h"
#include "SystemsLocator.h"

using namespace Toffee::Internal;

// Recieves a subsystem takes ownership via the relevant unique pointer.
void Toffee::ToffeeAPI::Provide(Subsystem* subsystem) {
	switch (subsystem->type_) {
	case SubsytemType::RENDER:
		render_system_ = std::unique_ptr<RenderSystem>(static_cast<RenderSystem*>(subsystem));
		break;
	case SubsytemType::AUDIO:
		audio_system_ = std::unique_ptr<AudioSystem>(static_cast<AudioSystem*>(subsystem));
		break;
	case SubsytemType::EVENT:
		event_system_ = std::unique_ptr<EventSystem>(static_cast<EventSystem*>(subsystem));
		break;
	case SubsytemType::WINDOW:
		window_system_ = std::unique_ptr<WindowSystem>(static_cast<WindowSystem*>(subsystem));
		break;
	case SubsytemType::INPUT:
		input_system_ = std::unique_ptr<InputSystem>(static_cast<InputSystem*>(subsystem));
		break;
	case SubsytemType::WORLD:
		world_system_ = std::unique_ptr<WorldSystem>(static_cast<WorldSystem*>(subsystem));
		break;
	case SubsytemType::RESOURCE:
		resource_system_ = std::unique_ptr<ResourceSystem>(static_cast<ResourceSystem*>(subsystem));
		break;
	case SubsytemType::UI:
		ui_system_ = std::unique_ptr<UserInterfaceSystem>(static_cast<UserInterfaceSystem*>(subsystem));
		break;
	case SubsytemType::PHYSICS:
		physics_system_ = std::unique_ptr<PhysicsSystem>(static_cast<PhysicsSystem*>(subsystem));
		break;

	default:
		std::cout << "\n\nError: Unknown subsytem\n\n";
		break;
	}
}

void Toffee::ToffeeAPI::InitialiseSubsystems() {
	ui_system_->Initialise();
	resource_system_->Initialise();
	audio_system_->Initialise();	
	window_system_->Initialise();
	render_system_->Initialise();
	event_system_->Initialise();
	input_system_->Initialise();
	world_system_->Initialise();
	physics_system_->Initialise();
}

void Toffee::ToffeeAPI::ShutDownSubsystems() {
	physics_system_->Shutdown();
	world_system_->Shutdown();
	input_system_->Shutdown();
	event_system_->Shutdown();
	render_system_->Shutdown();
	window_system_->Shutdown();
	audio_system_->Shutdown();
	resource_system_->Shutdown();
	ui_system_->Shutdown();
}
