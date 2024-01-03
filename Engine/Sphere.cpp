#include "pch.h"
#include "Sphere.h"

void Sphere::CreateSphere(Engine::Components::Mesh& mesh) {
	const double pi = 3.14159265358979323846;

	float thetaInterval = (pi * 2) / 100;
	float deltaInterval = pi / 100;
	float theta = 0;
	float delta = 0;

	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			//Point #1

			mesh.normals.push_back(cos(theta) * sin(delta));
			mesh.vertex.push_back(0.5 * mesh.normals.back());
			mesh.texCoords.push_back(-theta / (2 * pi));


			mesh.normals.push_back(cos(delta));
			mesh.vertex.push_back(0.5 * mesh.normals.back());
			mesh.texCoords.push_back((mesh.normals.back() - 1) / -2);

			mesh.vertex.push_back(0.5 * sin(theta) * sin(delta));
			mesh.normals.push_back(2 * mesh.vertex.back());

			//Point #2

			mesh.normals.push_back(cos(theta + thetaInterval) * sin(delta));
			mesh.vertex.push_back(0.5 * mesh.normals.back());
			mesh.texCoords.push_back(-(theta + thetaInterval) / (2 * pi));


			mesh.normals.push_back(cos(delta));
			mesh.vertex.push_back(0.5 * mesh.normals.back());
			mesh.texCoords.push_back((mesh.normals.back() - 1) / -2);

			mesh.vertex.push_back(0.5 * sin(theta + thetaInterval) * sin(delta));
			mesh.normals.push_back(2 * mesh.vertex.back());

			//Point #3

			mesh.normals.push_back(cos(theta + thetaInterval) * sin(delta + deltaInterval));
			mesh.vertex.push_back(0.5 * mesh.normals.back());
			mesh.texCoords.push_back(-(theta + thetaInterval) / (2 * pi));


			mesh.normals.push_back(cos(delta + deltaInterval));
			mesh.vertex.push_back(0.5 * mesh.normals.back());
			mesh.texCoords.push_back((mesh.normals.back() - 1) / -2);

			mesh.vertex.push_back(0.5 * sin(theta + thetaInterval) * sin(delta + deltaInterval));
			mesh.normals.push_back(2 * mesh.vertex.back());


			//Point #4

			mesh.normals.push_back(cos(theta) * sin(delta + deltaInterval));
			mesh.vertex.push_back(0.5 * mesh.normals.back());
			mesh.texCoords.push_back(-theta / (2 * pi));


			mesh.normals.push_back(cos(delta + deltaInterval));
			mesh.vertex.push_back(0.5 * mesh.normals.back());
			mesh.texCoords.push_back((mesh.normals.back() - 1) / -2);

			mesh.vertex.push_back(0.5 * sin(theta) * sin(delta + deltaInterval));
			mesh.normals.push_back(2 * mesh.vertex.back());


			theta += thetaInterval;
		}
		delta += deltaInterval;
		theta = 0;
	}
}
