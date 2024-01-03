#include "pch.h"
#include "Quad.h"


void Quad::CreateQuad(Engine::Components::Mesh& mesh) {
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
