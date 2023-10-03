#include "Grame.h"
#include "RenderSystem.h"
#include "AudioSystem.h"
#include <iostream>

//Create the application layer
//See Grapplication.h
Grengine::Grapplication* Grengine::CreateApp(int argc, char** argv)
{
	return new Grame();
}

Grame::Grame()
{
	Grengine::grAudioSystem->loadSound("");
}

void Grame::Update(double dt)
{

	std::cout << "Update " << dt << lorem[word] << "\n";
	word++;
	if (word > 9) word = 0;
}

void Grame::Render()
{
	Grengine::grRenderSystem->DrawLine(100, 200, 400, 700);
	Grengine::grRenderSystem->DrawLine(100, 200, 1200, 50);
	Grengine::grRenderSystem->DrawLine(1200, 50, 400, 700);
	//Needs to make calls to Grengine::grRenderSystem clear and display 
}
