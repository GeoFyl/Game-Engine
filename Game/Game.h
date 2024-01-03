#pragma once
#include <Engine.h>
#include <string>

class Game : public Engine::App::Application
{
public:
	Game();
	void Start() override final;
	void Update(double dt) override final;
	//void Render() override final;

private:

	int window_width_, window_height_;

	Engine::GameObject player_;
	Engine::GameObject obj_;
	UIElement_ID ui_info_;
	UIElement_ID ui_fps_;
	UIElement_ID ui_fps_count_;
	float fps_count_time_ = 0;

	std::string lorem[10] = { " Lorem", " ipsum", " dolor", " sit", " amet", " consectetur", " adipiscing", " elit", " sed", " do"};
	int word = 0;
};
