#include "pch.h"
#include "Quad.h"

// Adapted from my 2nd year CMP203 coursework.

void Quad::CreateQuad(Toffee::Components::Mesh& mesh) {
	mesh.vertex = std::move(std::vector<float>({
		-0.5,  0.5, 0,
		-0.5, -0.5, 0,
		0.5, -0.5, 0,
		0.5,  0.5, 0
	}));

	mesh.normals = std::move(std::vector<float>({
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0
	}));

	mesh.texCoords = std::move(std::vector<float>({
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0
	}));
}
