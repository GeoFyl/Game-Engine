#include "Game.h"
#include "RenderSystem.h"
#include <iostream>

//Create the application layer
//See Application.h
Engine::App::Application* Engine::App::CreateApp(int argc, char** argv)
{
	return new Game();
}

Game::Game()
{
}

void Game::Start() {
	/*Engine::audio_system_->loadSound("Gong", "Media/loser.wav");
	Engine::audio_system_->play("Gong");*/
	Engine::SystemsAPI::Audio()->LoadSound("Gong", "Media/loser.wav");
	Engine::SystemsAPI::Audio()->Play("Gong");
}

void Game::Update(double dt)
{
	/*std::cout << "Update " << dt << lorem[word] << "\n";
	word++;
	if (word > 9) word = 0;*/
}

void Game::Render()
{
	Engine::SystemsAPI::Renderer()->DrawLine(100, 200, 400, 700);
	Engine::SystemsAPI::Renderer()->DrawLine(100, 200, 1200, 50);
	Engine::SystemsAPI::Renderer()->DrawLine(1200, 50, 400, 700);
	//Needs to make calls to Engine::RenderSystem clear and display 
}
