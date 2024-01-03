#pragma once
#include "Components.h"

class Plane {
public:
	static void CreatePlane(Engine::Components::Mesh& mesh);

private:
	static void CropUV(float& coord);
};

