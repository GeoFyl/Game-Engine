#include "pch.h"
#include "SystemsLocator.h"

using namespace Engine::Internal;

void Engine::SystemsAPI::Provide(Subsystem* subsystem) {
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
	default:
		std::cout << "\n\nError: Unknown subsytem\n\n";
		break;
	}
}

void Engine::SystemsAPI::InitialiseSubsystems() {
	audio_system_.get()->Initialise();	
	window_system_.get()->Initialise();
	render_system_.get()->Initialise();

	event_system_.get()->Initialise();
}

void Engine::SystemsAPI::ShutDownSubsystems() {
	audio_system_.get()->Shutdown();
	render_system_.get()->Shutdown();

	event_system_.get()->Shutdown();
	window_system_.get()->Shutdown();
}
