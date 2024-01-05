#pragma once
#include <ToffeeEngine.h>

class Game : public Toffee::App::Application
{
public:
	// Required by the engine. Perform any initializations here.
	void Start() override final;

	// Required by the engine. Called every frame, your game logic goes here.
	///<param name="dt">Time since the last Update</param>
	void Update(double dt) override final;
	
private:
	// Your own functions and member variables should go here.

	void FinishSetup();
	void ProcessInput();

	Toffee::GameObject player_;
	Toffee::GameObject pot_;
	Toffee::UIElement_ID ui_info_ = -1;
	Toffee::UIElement_ID ui_fps_ = -1;
	Toffee::UIElement_ID ui_fps_count_ = -1;
	Toffee::UIElement_ID ui_loading_ = -1;
	float fps_count_time_ = 0;

	bool loaded_ = false;
};

//Create the application layer
//See Application.h
Toffee::App::Application* Toffee::App::CreateApp(int argc, char** argv)
{
	return new Game();
}