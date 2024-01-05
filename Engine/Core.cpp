#include "pch.h"
#include "Core.h"
#include "SystemsLocator.h"

typedef std::chrono::high_resolution_clock Clock;

Core::Core(int argc, char** argv) :	app_{Toffee::App::CreateApp(argc, argv)} //Create the application layer
{
	//Initialise Subsystems
	ToffeeAPI::InitialiseSubsystems();
}

//Main loop
int Core::Run() {

	// Initialise the application
	app_->Start();

	// Start the time system
	float dt = 0.f;
	ToffeeAPI::Time()->Start();

	//Main loop
	while (1) {
		//Events
		if (ProcessEvents() == -1) {
			break;
		}

		// Update
		if (Update(dt) == -1) { // Returns -1 when user has requested to quit
			break;
		}

		//Render 
		Render();
	}

	Close();
	return 0;
}

int Core::ProcessEvents() {
	return ToffeeAPI::Events()->ProcessEvents();
}

int Core::Update(double dt) {
	// Update subsystems
	ToffeeAPI::Input()->Update();
	ToffeeAPI::Resources()->Update();
	dt = ToffeeAPI::Time()->Update();
	if (ToffeeAPI::Physics()->Update(dt)) {
		ToffeeAPI::World()->Update();
	}

	// Update application
	app_->Update(dt);
	return app_->GetExitState();
}

void Core::Render() {		
	ToffeeAPI::Renderer()->Clear();
	ToffeeAPI::Renderer()->Render();
	ToffeeAPI::Renderer()->Display();
}

void Core::Close() {
	ToffeeAPI::ShutDownSubsystems();
}
