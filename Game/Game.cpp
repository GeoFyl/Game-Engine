#include "Game.h"
#include "RenderSystem.h"
#include <iostream>
 
typedef Engine::SystemsAPI sys;

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
	sys::Window()->OpenWindow("Very cool game", 1280, 720);
	window_width_ = sys::Window()->GetWidth();
	window_height_ = sys::Window()->GetHeight();

	player_ = sys::World()->CreateGameObject(-2, 0, 0);
	//sys::World()->SetScale(player_, 0.1, 0.1, 0.1);

	Engine::GameObject light = sys::World()->CreateSpotLight(-3.5, 1, 0, 1, -1, 0);
	
	//sys::World()->SetDiffuseSource(light, 1, 0, 0, 1);
	//sys::World()->SetAmbientSource(light, 0.5, 0, 0, 1);

	sys::Resources()->SetArchive("arc.zip");
	sys::Resources()->Mount("arc", "Resources");
	sys::Resources()->Mount("Media", "Resources");
	sys::Resources()->Load("Resources/test.txt", "ATest");
	sys::Resources()->Load("Resources/loser.wav", "Gong");
	sys::Resources()->Load("Resources/teapot.obj", "Teapot");
	sys::Resources()->Load("Resources/ball.png", "BallTex");
	sys::Resources()->Load("Resources/wood.png", "WoodTex");
	sys::Resources()->Load("Resources/crate.png", "CrateTex");
	sys::Resources()->Load("Resources/logosmall.png", "Logo");
	sys::Resources()->Load("Resources/Digital1.ttf", "DigitalFont");
	while (sys::Resources()->Loading()) {
		printf("Loading...\n");
	}
	sys::Audio()->Play("Gong");
	
	sys::World()->SetMesh(player_, "Sphere");
	//sys::World()->SetMesh(obj_, "Teapot");
	sys::World()->SetTexture(player_, "BallTex");

	sys::Physics()->AddSphereBody(player_, 0.5, Engine::PhysicsType::DYNAMIC, 1, 0.6);

	//sys::World()->SetScale(player_, 5, 5, 5);
	sys::Physics()->SetAngularDamping(player_, 1.2);

	Engine::GameObject plane = sys::World()->CreateGameObject(0, -2, -5);
	sys::World()->SetMesh(plane, "Plane");
	sys::World()->SetTexture(plane, "WoodTex");
	sys::World()->SetScale(plane, 20, 1, 25);
	sys::Physics()->AddPlaneBody(plane);

	Engine::GameObject cube = sys::World()->CreateGameObject(-2.5, -1.625, 0);
	sys::World()->SetMesh(cube, "Cube");
	sys::World()->SetTexture(cube, "CrateTex");
	sys::Physics()->AddBoxBody(cube, 0.5, 0.5, 0.5, Engine::PhysicsType::STATIC);
	sys::World()->SetScale(cube, 0.75, 0.75, 2);



	//sys::World()->SetAmbientMat(player_, 0, 0, 1, 1);
	//sys::World()->SetDiffuseMat(player_, 1, 0, 0, 1);
	ui_fps_ = sys::UI()->AddText(155, 10, 305, 18, "DigitalFont", "Click here to enable/disable VSync. FPS:");
	ui_fps_count_ = sys::UI()->AddText(330, 10, 200, 18, "DigitalFont", "60");
	ui_info_ = sys::UI()->AddText(255, 100, 500, 18, "DigitalFont", "Controls:\n- Right-click to start/pause simulation\n- WASD to roll ball\n- Q and E to rotate\n- Space to jump\n- F to toggle wireframe mode\n- L to toggle lighting\n- Click on this text, cursor should warp to centre of screen");
	sys::UI()->AddElement(sys::Window()->GetWidth() - 200, sys::Window()->GetHeight() - 150, 250, 250, "Logo");
	//sys::UI()->AddElement(200, 600, 200, 100, "WoodTex");
	//ui_elem_ = sys::UI()->AddElement(200, 100, 200, 100, "WoodTex");
}

void Game::Update(double dt)
{
	if (sys::Input()->IsPressed(Engine::Button::ESCAPE)) {
		Exit();
	}
	fps_count_time_ += dt;
	if (fps_count_time_ >= 0.5) {
		fps_count_time_ = 0;
		sys::UI()->SetText(ui_fps_count_, 200, 18, "DigitalFont", std::to_string(static_cast<int>(sys::Time()->GetAvgFPS())));
	}

	if (sys::Input()->IsPressed(Engine::Button::MOUSE_LEFT) && sys::UI()->IsMouseOver(ui_fps_)) {
		sys::Renderer()->ToggleVsync();
	}
	if (sys::Input()->IsPressed(Engine::Button::MOUSE_LEFT) && sys::UI()->IsMouseOver(ui_info_)) {
		sys::Input()->WarpMouse(window_width_ / 2, window_height_ / 2);
	}
	if (sys::Input()->IsPressed(Engine::Button::MOUSE_RIGHT)) {
		sys::Physics()->ToggleSimulating();
	}
	if (sys::Input()->IsPressed(Engine::Button::MOUSE_WHEEL)) {
		std::cout << "Pressed m wheel\n";
	}
	if (sys::Input()->IsPressed(Engine::Button::MOUSE_WHEEL_UP)) {
		std::cout << "m wheel up\n";
	}
	if (sys::Input()->IsPressed(Engine::Button::MOUSE_WHEEL_DOWN)) {
		std::cout << "m wheel down\n";
	}

	if (sys::Input()->IsDown(Engine::Button::A) || sys::Input()->IsDown(Engine::Button::D) || sys::Input()->IsDown(Engine::Button::W) || sys::Input()->IsDown(Engine::Button::S)) {
		sys::Physics()->SetVelocity(player_, 0, 0, 0);
		if (sys::Input()->IsDown(Engine::Button::A)) {
			//sys::World()->Move(player_, -6 * dt, 0, 0);
			sys::Physics()->ApplyVelocity(player_, -6, 0, 0);
			//std::cout << "Player X: " << sys::World()->GetObjectPosition(player_).x << "\n";
		}
		if (sys::Input()->IsDown(Engine::Button::D)) {
			sys::Physics()->ApplyVelocity(player_, 6, 0, 0);
			//std::cout << "Player X: " << sys::World()->GetObjectPosition(player_).x << "\n";
		}
		if (sys::Input()->IsDown(Engine::Button::W)) {
			//sys::World()->Move(player_, 0, 0, -6 * dt);
			sys::Physics()->ApplyVelocity(player_, 0, 0, -6);
			//std::cout << "Player X: " << sys::World()->GetObjectPosition(player_).x << "\n";
		}
		if (sys::Input()->IsDown(Engine::Button::S)) {
			sys::Physics()->ApplyVelocity(player_, 0, 0, 6);
			//sys::World()->Move(player_, 0, 0, 6 * dt);
			//std::cout << "Player X: " << sys::World()->GetObjectPosition(player_).x << "\n";
		}
	}
	else if (sys::Input()->IsReleased(Engine::Button::A) || sys::Input()->IsReleased(Engine::Button::D) || sys::Input()->IsReleased(Engine::Button::W) || sys::Input()->IsReleased(Engine::Button::S)) {
		sys::Physics()->SetVelocity(player_, 0, 0, 0);
	}
	if (sys::Input()->IsDown(Engine::Button::Q)) {
		sys::World()->Rotate(player_, 0, 90, 0);
		//std::cout << "Player X: " << sys::World()->GetObjectPosition(player_).x << "\n";
	}
	if (sys::Input()->IsDown(Engine::Button::E)) {
		sys::World()->Rotate(player_, 0, -90, 0);
		//std::cout << "Player X: " << sys::World()->GetObjectPosition(player_).x << "\n";
	}
	if (sys::Input()->IsPressed(Engine::Button::SPACE)) {
		sys::Physics()->ApplyImpulse(player_, 0, 2, 0);
		//std::cout << "Player X: " << sys::World()->GetObjectPosition(player_).x << "\n";
	}
	if (sys::Input()->IsPressed(Engine::Button::F)) {
		sys::Renderer()->ToggleWireframe();
		//std::cout << "Player X: " << sys::World()->GetObjectPosition(player_).x << "\n";
	}
	if (sys::Input()->IsPressed(Engine::Button::L)) {
		sys::Renderer()->ToggleLighting();
		//std::cout << "Player X: " << sys::World()->GetObjectPosition(player_).x << "\n";
	}


	/*std::cout << "Update " << dt << lorem[word] << "\n";
	word++;
	if (word > 9) word = 0;*/
}
