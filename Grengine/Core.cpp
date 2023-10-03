#include "pch.h"
#include "Core.h"
#include "RenderSystem.h"
#include "EventSystem.h"
#include "AudioSystem.h"

namespace Grengine {

	Core::Core(int argc, char** argv) :
		grApp{Grengine::CreateApp(argc, argv)}, //Create the application layer
		grTimeSystem{ 60 }						//Targeting 60 frames per second
	{
		//Initialise Subsystems
		grRenderSystem->Initialise();   //global pointer grRenderSystem is created in RenderSystem.h
		grEventSystem->Initialise();	//global pointer grEventSystem is created in EventSystem.h
		grAudioSystem->Initialise();
	}

	void Core::OpenWindow(int ScreenWidth, int ScreenHeight) {
		grRenderSystem->OpenWindow(ScreenWidth, ScreenHeight); 
		grRenderSystem->CreateRenderer();
	}

	//Our main loop
	int Core::Run() {

		//Maybe the application layer should be in charge of calling this?
		OpenWindow(1280, 720);

		//Main loop
		while (1) {
			//Events
			if (ProcessEvents() == -1) {
				break;
			}

			//Timing			
			grTimeSystem.Tick();

			//Work out whether we need to call Update this tick and, if so, how many times (usually 0 or 1)
			int updateCount = grTimeSystem.GetUpdateCount();

			//Frame
			if (updateCount > 0) {
				double dt = 0;	//dt == DeltaTime in seconds: How long since the last Update

				auto deltaTime = grTimeSystem.Update(); //Get Delta Time in nanoseconds

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
		return grEventSystem->ProcessEvents();
	}
	void Core::Update(double dt)
	{
		grApp->Update(dt);
	}
	void Core::Render()
	{		
		//Somebody took my renderer!
		grRenderSystem->Clear();
		grApp->Render();
		grRenderSystem->Display();
		//Should call grApp->Render(), which will in turn make calls to Grengine::grRenderSystem interface
	}
	void Core::Close()
	{
		grEventSystem->Shutdown();
		grRenderSystem->Shutdown();
		grAudioSystem->Shutdown();
	}
}