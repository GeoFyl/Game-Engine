#include "Game.h"
 
typedef Toffee::ToffeeAPI API;

void Game::Start() {
	// Open window, create scene (and set it as the current scene)
	API::Window()->OpenWindow("Very cool game", 1280, 720);
	API::World()->SwitchScene("Scene1");

	// Select arc.toff as the current archive for loading assets and mount the arc subfolder within to the virtual location 'Resources'
	API::Resources()->SetArchive("arc.toff");
	API::Resources()->Mount("arc", "Resources");

	// Mount a local folder 'Media' to the virtual location 'Resources'
	API::Resources()->Mount("Media", "Resources");

	// Load font synchronously (blocking) and create UI text element for loading screen
	API::Resources()->Load("Resources/Digital1.ttf", "DigitalFont", false);
	ui_loading_ = API::UI()->AddText(API::Window()->GetWidth() /2, API::Window()->GetHeight() /2, 500, 50, "DigitalFont", "Loading...");

	// Begin loading other resources asynchronously
	API::Resources()->Load("Resources/Instructions.txt", "Instructions");
	API::Resources()->Load("Resources/music.mp3", "Music");
	API::Resources()->Load("Resources/gong.wav", "Gong");
	API::Resources()->Load("Resources/ball.png", "BallTex");
	API::Resources()->Load("Resources/wood.png", "WoodTex");
	API::Resources()->Load("Resources/crate1.jpg", "CrateTex");
	API::Resources()->Load("Resources/logosmall.png", "Logo");
}

void Game::Update(double dt)
{
	// User can quit with esc
	if (API::Input()->IsPressed(Toffee::Button::ESCAPE)) {
		Exit();
	}

	// Every half second retrieve and display the average FPS
	fps_count_time_ += dt;
	if (fps_count_time_ >= 0.5) {
		fps_count_time_ = 0;
		API::UI()->SetText(ui_fps_count_, 200, 18, "DigitalFont", std::to_string(static_cast<int>(API::Time()->GetAvgFPS())), 0.447, 1, 0.961); // Here we're giving it a colour
	}
	
	// Resources are still loading so I want to just show loading screen (in this case do nothing, but could easily be fancier)
	if (API::Resources()->Loading()) return;
	
	// Logic for scene 1
	if (API::World()->GetScene() == "Scene1") {

		// If we've just finished loading the resources, set up the scene
		if (!loaded_ && !API::Resources()->Loading()) FinishSetup();

		// Main logic
		ProcessInput();
		if (API::Physics()->Simulating()) { //Only want to add forces etc while physics simulation is running
			if (API::Input()->IsDown(Toffee::Button::A) || API::Input()->IsDown(Toffee::Button::D) || API::Input()->IsDown(Toffee::Button::W) || API::Input()->IsDown(Toffee::Button::S)) {
				API::Physics()->SetVelocity(player_, 0, 0, 0);
				if (API::Input()->IsDown(Toffee::Button::A)) {
					API::Physics()->ApplyVelocity(player_, -6, 0, 0);
				}
				if (API::Input()->IsDown(Toffee::Button::D)) {
					API::Physics()->ApplyVelocity(player_, 6, 0, 0);
				}
				if (API::Input()->IsDown(Toffee::Button::W)) {
					API::Physics()->ApplyVelocity(player_, 0, 0, -6);
				}
				if (API::Input()->IsDown(Toffee::Button::S)) {
					API::Physics()->ApplyVelocity(player_, 0, 0, 6);
				}
			}
			else if (API::Input()->IsReleased(Toffee::Button::A) || API::Input()->IsReleased(Toffee::Button::D) || API::Input()->IsReleased(Toffee::Button::W) || API::Input()->IsReleased(Toffee::Button::S)) {
				API::Physics()->SetVelocity(player_, 0, 0, 0);
			}
			if (API::Input()->IsPressed(Toffee::Button::SPACE)) {
				API::Physics()->ApplyImpulse(player_, 0, 2, 0);
			}
		}
		if (API::Input()->IsDown(Toffee::Button::Q)) {
			API::World()->Rotate(player_, 0, 90 * dt, 0);
		}
		if (API::Input()->IsDown(Toffee::Button::E)) {
			API::World()->Rotate(player_, 0, -90 * dt, 0);
		}
	}

	// Logic for scene 2
	else if(API::World()->GetScene() == "Scene2") {

		// Set up the scene if just finished loading resources
		if (!loaded_ && !API::Resources()->Loading()) {
			loaded_ = true;
			API::UI()->SetVisible(ui_loading_, false);
			
			// Create a directional light whose diffuse component is red and ambient is orange
			Toffee::GameObject light = API::World()->CreateDirectionalLight(1, 0.5, 0);
			API::World()->SetDiffuseSource(light, 0.8, 0, 0, 1);
			API::World()->SetAmbientSource(light, 0.98, 0.522, 0, 1);

			// Create object with whose material ambient component is light blue 
			pot_ = API::World()->CreateGameObject(0, 0, -3);
			API::World()->SetMesh(pot_, "Teapot");
			API::World()->SetAmbientMat(pot_, 0.447, 0.7, 0.961, 0.5);
			API::World()->SetTexture(pot_, "StoneTex");
			API::World()->SetScale(pot_, 0.25, 0.25, 0.25);
		}

		// User input
		ProcessInput();

		// Rotate the object
		API::World()->Rotate(pot_, 0, 90 * dt, 0);
	}
}

void Game::FinishSetup() {
	// Start playing music 
	API::Audio()->Play("Music");

	// Set up lighting and objects in the current (1st) scene
	Toffee::GameObject light = API::World()->CreateSpotLight(-3.5, 1, 0, 1, -1, 0);
	player_ = API::World()->CreateGameObject(-2, 0, 0);
	API::World()->SetMesh(player_, "Sphere");
	API::World()->SetTexture(player_, "BallTex");
	API::Physics()->AddSphereBody(player_, 0.5, Toffee::PhysicsType::DYNAMIC, 1, 0.6);
	API::Physics()->SetAngularDamping(player_, 1.2);
	Toffee::GameObject plane = API::World()->CreateGameObject(0, -2, -5);
	API::World()->SetMesh(plane, "Plane");
	API::World()->SetTexture(plane, "WoodTex");
	API::World()->SetScale(plane, 20, 1, 25);
	API::Physics()->AddPlaneBody(plane);
	Toffee::GameObject cube = API::World()->CreateGameObject(-2.5, -1.625, 0);
	API::World()->SetMesh(cube, "Cube");
	API::World()->SetTexture(cube, "CrateTex");
	API::Physics()->AddBoxBody(cube, 0.5, 0.5, 0.5, Toffee::PhysicsType::STATIC);
	API::World()->SetScale(cube, 0.75, 0.75, 2);

	// Set up the UI
	ui_fps_ = API::UI()->AddText(155, 10, 305, 18, "DigitalFont", "Click here to enable/disable VSync. FPS:");
	ui_fps_count_ = API::UI()->AddText(330, 10, 200, 18, "DigitalFont", "60", true, 0.447, 1, 0.961);
	ui_info_ = API::UI()->AddText(255, 140, 500, 18, "DigitalFont", *reinterpret_cast<std::string*>(API::Resources()->GetResource("Instructions"))); // Retrieves text previously loaded
	API::UI()->AddElement(API::Window()->GetWidth() - 200, API::Window()->GetHeight() - 150, 250, 250, "Logo");

	API::UI()->SetVisible(ui_loading_, false);
	loaded_ = true;
}

void Game::ProcessInput() {
	// Play a sound when enter pressed
	if (API::Input()->IsPressed(Toffee::Button::ENTER)) {
		API::Audio()->Play("Gong");
	}

	// If the UI for vsync is clicked, toggle vsync
	if (API::Input()->IsPressed(Toffee::Button::MOUSE_LEFT) && API::UI()->IsMouseOver(ui_fps_)) {
		API::Renderer()->ToggleVsync();
	}

	// If the instruction text is clicked warp mouse pointer to centre of the window
	if (API::Input()->IsPressed(Toffee::Button::MOUSE_LEFT) && API::UI()->IsMouseOver(ui_info_)) {
		API::Input()->WarpMouse(API::Window()->GetWidth() / 2, API::Window()->GetHeight() / 2);
	}

	// Start/pause physics simulation on right-click
	if (API::Input()->IsPressed(Toffee::Button::MOUSE_RIGHT)) {
		API::Physics()->ToggleSimulating();
	}

	// Report mouse wheel activity
	if (API::Input()->IsPressed(Toffee::Button::MOUSE_WHEEL)) {
		std::cout << "Pressed m wheel\n";
	}
	if (API::Input()->IsPressed(Toffee::Button::MOUSE_WHEEL_UP)) {
		std::cout << "m wheel up\n";
	}
	if (API::Input()->IsPressed(Toffee::Button::MOUSE_WHEEL_DOWN)) {
		std::cout << "m wheel down\n";
	}

	// Toggle wireframe and lighting
	if (API::Input()->IsPressed(Toffee::Button::F)) {
		API::Renderer()->ToggleWireframe();
	}
	if (API::Input()->IsPressed(Toffee::Button::L)) {
		API::Renderer()->ToggleLighting();
	}

	// Switch camera position
	if (API::Input()->IsPressed(Toffee::Button::C)) {
		API::Renderer()->SetLookAt(6, 0, 0, 0, 0, 0); // Camera at (6,0,0), looking at (0,0,0)
	}
	if (API::Input()->IsPressed(Toffee::Button::V)) {
		API::Renderer()->SetLookAt(0, 0, 6, 0, 0, 0); // Camera at (0,0,6), looking at (0,0,0)
	}

	// Switch scenes 
	if (API::Input()->IsPressed(Toffee::Button::NUM_1)) {
		// Set scene 1 as current scene
		API::World()->SwitchScene("Scene1");
		API::Audio()->Play("Music");
	}
	if (API::Input()->IsPressed(Toffee::Button::NUM_2)) {
		API::Audio()->Stop("Music");

		// If scene 2 hasn't been set up yet
		if (!API::World()->SceneExists("Scene2")) {
			// Create scene 2 (and set it as the current scene)
			API::World()->SwitchScene("Scene2");

			// Start loading resources asynchronously
			API::UI()->SetVisible(ui_loading_, true);
			loaded_ = false;
			API::Resources()->Load("Resources/teapot2.obj", "Teapot");
			API::Resources()->Load("Resources/stone.jpg", "StoneTex");
		}
		// Set scene 2 as current scene
		else API::World()->SwitchScene("Scene2");
	}
}
