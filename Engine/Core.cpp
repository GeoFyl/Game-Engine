#include "pch.h"
#include "Core.h"
#include "SystemsLocator.h"

typedef std::chrono::high_resolution_clock Clock;

Core::Core(int argc, char** argv) :	app_{Engine::App::CreateApp(argc, argv)} //Create the application layer
{
	//Initialise Subsystems
	//render_system_->Initialise();   //global pointer grRenderSystem is created in RenderSystem.h
	//event_system_->Initialise();	//global pointer grEventSystem is created in EventSystem.h
	SystemsAPI::InitialiseSubsystems();
	/*audio_system_ = std::make_unique<AudioSystem_SoLoud>();
	audio_system_.get()->Initialise();
	SystemsAPI::Provide(audio_system_.get());*/
}

//void Core::OpenWindow(std::string name, int ScreenWidth, int ScreenHeight) {
//	SystemsAPI::Window()->OpenWindow(name, ScreenWidth, ScreenHeight);
//	SystemsAPI::Renderer()->CreateRenderer();
//}

//Our main loop
int Core::Run() {

	//Maybe the application layer should be in charge of calling this?
	//OpenWindow("Very cool game", 1280, 720);
	app_->Start();

	float dt = 0.f;
	SystemsAPI::Time()->Start();

	//Main loop
	while (1) {
		//Events
		if (ProcessEvents() == -1) {
			break;
		}

		SystemsAPI::Resources()->Update();

		dt = SystemsAPI::Time()->Update();

		if (SystemsAPI::Physics()->Update(dt)) {
			SystemsAPI::World()->Update();
		}


		if (Update(dt) == -1) {
			break;
		}

		//Render as fast as we can
		Render();
	}
	Close();
	return 0;
}
int Core::ProcessEvents() {
	return SystemsAPI::Events()->ProcessEvents();
}

int Core::Update(double dt) {
	SystemsAPI::Input()->Update();
	app_->Update(dt);
	return app_->GetExitState();
}

void Core::Render() {		
	SystemsAPI::Renderer()->Clear();
	SystemsAPI::Renderer()->Render();
	SystemsAPI::Renderer()->Display();
}

void Core::Close() {
	SystemsAPI::ShutDownSubsystems();
}
