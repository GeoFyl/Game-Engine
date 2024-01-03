#include "pch.h"
#include "Plane.h"

void Plane::CreatePlane(Engine::Components::Mesh& mesh) {
	// Creates 400 by 400 grid
	for (int x = -200; x < 200; x++) {
		for (int z = -200; z < 200; z++) {
			// Vertex 1
			mesh.vertex.push_back(x * 0.0025);
			mesh.texCoords.push_back(mesh.vertex.back() + 0.5);
			CropUV(mesh.texCoords.back());
			mesh.vertex.push_back(0);
			mesh.vertex.push_back(z * 0.0025);
			mesh.texCoords.push_back(mesh.vertex.back() + 0.5);
			CropUV(mesh.texCoords.back());

			mesh.normals.push_back(0);
			mesh.normals.push_back(1);
			mesh.normals.push_back(0);

			// Vertex 2
			mesh.vertex.push_back(x * 0.0025);
			mesh.texCoords.push_back(mesh.vertex.back() + 0.5);
			CropUV(mesh.texCoords.back());
			mesh.vertex.push_back(0);
			mesh.vertex.push_back(z * 0.0025 + 0.0025);
			mesh.texCoords.push_back(mesh.vertex.back() + 0.5);
			CropUV(mesh.texCoords.back());

			mesh.normals.push_back(0);
			mesh.normals.push_back(1);
			mesh.normals.push_back(0);

			// Vertex 3
			mesh.vertex.push_back(x * 0.0025 + 0.0025);
			mesh.texCoords.push_back(mesh.vertex.back() + 0.5);
			CropUV(mesh.texCoords.back());
			mesh.vertex.push_back(0);
			mesh.vertex.push_back(z * 0.0025 + 0.0025);
			mesh.texCoords.push_back(mesh.vertex.back() + 0.5);
			CropUV(mesh.texCoords.back());

			mesh.normals.push_back(0);
			mesh.normals.push_back(1);
			mesh.normals.push_back(0);

			// Vertex 4
			mesh.vertex.push_back(x * 0.0025 + 0.0025);
			mesh.texCoords.push_back(mesh.vertex.back() + 0.5);
			CropUV(mesh.texCoords.back());
			mesh.vertex.push_back(0);
			mesh.vertex.push_back(z * 0.0025);
			mesh.texCoords.push_back(mesh.vertex.back() + 0.5);
			CropUV(mesh.texCoords.back());

			mesh.normals.push_back(0);
			mesh.normals.push_back(1);
			mesh.normals.push_back(0);


		}
	}
}

void Plane::CropUV(float& coord) {
	// Crop edges of plane
	if (coord == 0) coord += 0.005;
	else if (coord == 1) coord -= 0.005;
}
