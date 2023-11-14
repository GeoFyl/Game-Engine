#pragma once
#include <Engine.h>
#include <string>

class Game : public Engine::App::Application
{
public:
	Game();
	void Start() override final;
	void Update(double dt) override final;
	void Render() override final;

private:

	std::string lorem[10] = { " Lorem", " ipsum", " dolor", " sit", " amet", " consectetur", " adipiscing", " elit", " sed", " do"};
	int word = 0;
};
