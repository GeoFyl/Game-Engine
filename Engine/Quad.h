#pragma once
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Quad {
public:
	Quad(float width, float height);
	void render();

protected:
	float* vertex;
	float* normals;
	float* texCoords;
};

