#pragma once
#include "Components.h"

// Adapted from my 2nd year CMP203 coursework.

// Constructs a plane mesh.
class Plane {
public:
	static void CreatePlane(Toffee::Components::Mesh& mesh);

private:
	static void CropUV(float& coord);
};

