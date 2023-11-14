#include "pch.h"
#include "Core.h"
#include "RenderSystem.h"
#include "EventSystem.h"
#include "AudioSystem_SoLoud.h"
#include "SystemsLocator.h"

Core::Core(int argc, char** argv) :
	app_{Engine::App::CreateApp(argc, argv)}, //Create the application layer
	time_system_{ 60 }						//Targeting 60 frames per second
{
	//Initialise Subsystems
	//render_system_->Initialise();   //global pointer grRenderSystem is created in RenderSystem.h
	//event_system_->Initialise();	//global pointer grEventSystem is created in EventSystem.h
	SystemsAPI::InitialiseSubsystems();
	/*audio_system_ = std::make_unique<AudioSystem_SoLoud>();
	audio_system_.get()->Initialise();
	SystemsAPI::Provide(audio_system_.get());*/
}

void Core::OpenWindow(std::string name, int ScreenWidth, int ScreenHeight) {
	SystemsAPI::Window()->OpenWindow(name, ScreenWidth, ScreenHeight);
	SystemsAPI::Renderer()->CreateRenderer();
}

//Our main loop
int Core::Run() {

	//Maybe the application layer should be in charge of calling this?
	OpenWindow("Very cool game", 1280, 720);
	app_->Start();

	//Main loop
	while (1) {
		//Events
		if (ProcessEvents() == -1) {
			break;
		}

		//Timing			
		time_system_.Tick();

		//Work out whether we need to call Update this tick and, if so, how many times (usually 0 or 1)
		int updateCount = time_system_.GetUpdateCount();

		//Frame
		if (updateCount > 0) {
			double dt = 0;	//dt == DeltaTime in seconds: How long since the last Update

			auto deltaTime = time_system_.Update(); //Get Delta Time in nanoseconds

			//Handle the possibility that there are multiple frames to run
			dt = std::chrono::duration_cast<std::chrono::duration<double, std::chrono::seconds::period>>(deltaTime).count();
			dt /= updateCount;

			while (updateCount > 0) {
				//Update on a fixed timescale
				Update(dt);
				updateCount--;
			}
		}

		//Render as fast as we can
		Render();
	}
	Close();
	return 0;
}
int Core::ProcessEvents()
{
	return SystemsAPI::Events()->ProcessEvents();
}
void Core::Update(double dt)
{
	app_->Update(dt);
}
void Core::Render()
{		
	SystemsAPI::Renderer()->Clear();
	SystemsAPI::Renderer()->Render();
	SystemsAPI::Renderer()->Display();
}
void Core::Close()
{
	SystemsAPI::ShutDownSubsystems();
}
