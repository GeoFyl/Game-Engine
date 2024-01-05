#include "pch.h"
#include "WorldSystem_EnTT.h"
#include "SystemsLocator.h"
#include <PxPhysicsAPI.h>

using namespace Toffee::Internal;
using namespace Toffee::Components;

// Self register
WorldSystem_EnTT* world_entt = new WorldSystem_EnTT;
WorldSystem_EnTT::WorldSystem_EnTT() {
	ProvideSystem(this);
	world_entt = nullptr;
}

int WorldSystem_EnTT::Shutdown()
{
	for (auto& scene : scenes_) {
		scene.second.clear();
	}
	scenes_.clear();
	return true;
}

// Create an empty game object at (0,0,0).
Toffee::GameObject WorldSystem_EnTT::CreateGameObject()
{
	return CreateGameObject(0, 0, 0);
}

// Create an empty game object at (x,y,z).
Toffee::GameObject WorldSystem_EnTT::CreateGameObject(float x, float y, float z)
{
	auto entity = scenes_[current_scene_].create();
	auto& transform = scenes_[current_scene_].emplace<Transform>(entity);
	// Set the transformation matrix to a translation matrix using x,y,z
	CopyMatrix(transform.matrix, physx::PxMat44(physx::PxMat33(physx::PxIdentity), physx::PxVec3(x, y, z)).front());
	return (GameObject)entity;
}

// Create directional light pointing in direction (1,0,0).
Toffee::GameObject WorldSystem_EnTT::CreateDirectionalLight() {
	return CreateDirectionalLight(1, 0, 0);
}

// Create directional light pointing in direction (x,y,z).
Toffee::GameObject WorldSystem_EnTT::CreateDirectionalLight(float x, float y, float z) {
	int num_lights = scenes_[current_scene_].view<Light>().size();
	if (num_lights >= 8) {
		printf("Max lights reached. Cannot create any more lights in scene\n");
		return -1;
	}
	auto entity = scenes_[current_scene_].create();
	auto& transform = scenes_[current_scene_].emplace<Transform>(entity);
	CopyMatrix(transform.matrix, physx::PxMat44(physx::PxMat33(physx::PxIdentity), physx::PxVec3(-x, -y, -z)).front());
	auto& light = scenes_[current_scene_].emplace<Light>(entity);
	light.has_position = false;
	return (GameObject)entity;
}

// Create a point light at (0,0,0).
Toffee::GameObject WorldSystem_EnTT::CreatePointLight() {
	return CreatePointLight(0, 0, 0);
}

// Create a point light at (x,y,z).
Toffee::GameObject WorldSystem_EnTT::CreatePointLight(float x, float y, float z) {
	int num_lights = scenes_[current_scene_].view<Light>().size();
	if (num_lights >= 8) {
		printf("Max lights reached. Cannot create any more lights in scene\n");
		return -1;
	}
	auto entity = scenes_[current_scene_].create();
	auto& transform = scenes_[current_scene_].emplace<Transform>(entity);
	CopyMatrix(transform.matrix, physx::PxMat44(physx::PxMat33(physx::PxIdentity), physx::PxVec3(x, y, z)).front());
	scenes_[current_scene_].emplace<Light>(entity);
	return (GameObject)entity;
}

// Create a spot light at (0,0,0) pointing straight down.
Toffee::GameObject WorldSystem_EnTT::CreateSpotLight(float x, float y, float z, float d_x, float d_y, float d_z) {
	int num_lights = scenes_[current_scene_].view<Light>().size();
	if (num_lights >= 8) {
		printf("Max lights reached. Cannot create any more lights in scene\n");
		return -1;
	}
	auto entity = scenes_[current_scene_].create();
	auto& transform = scenes_[current_scene_].emplace<Transform>(entity);
	CopyMatrix(transform.matrix, physx::PxMat44(physx::PxMat33(physx::PxIdentity), physx::PxVec3(x, y, z)).front());
	Light& light = scenes_[current_scene_].emplace<Light>(entity);
	light.spot_dir[0] = d_x;
	light.spot_dir[1] = d_y;
	light.spot_dir[2] = d_z;
	light.spot_cutoff = 50.f;
	light.spot_exp = 20.f;
	return (GameObject)entity;
}

// Create a spot light at (0,0,0) pointing in direction (x,y,z).
Toffee::GameObject WorldSystem_EnTT::CreateSpotLight() {
	return CreateSpotLight(0, 0, 0, 0, -1, 0);
}

// Translate an object's position by (x,y,z) amount.
void WorldSystem_EnTT::Move(GameObject entity, float x, float y, float z) {
	auto& transform = scenes_[current_scene_].get<Transform>(static_cast<entt::entity>(entity));
	transform.matrix[12] += x;
	transform.matrix[13] += y;
	transform.matrix[14] += z;
	PhysicsBody* body_component = scenes_[current_scene_].try_get<PhysicsBody>(static_cast<entt::entity>(entity));
	if (body_component) {
		physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(scenes_[current_scene_].try_get<PhysicsBody>(static_cast<entt::entity>(entity))->body);
		physx::PxTransform t = phys_body->getGlobalPose();
		t.p += physx::PxVec3(x, y, z);
		phys_body->setGlobalPose(t);
	}
}

// Rotate an object around its local axis.
void WorldSystem_EnTT::Rotate(GameObject entity, float rotx, float roty, float rotz) {
	auto& transform = scenes_[current_scene_].get<Transform>(static_cast<entt::entity>(entity));
	PhysicsBody* body_component = scenes_[current_scene_].try_get<PhysicsBody>(static_cast<entt::entity>(entity));
	physx::PxTransform t;
	if (body_component) {
		physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(scenes_[current_scene_].try_get<PhysicsBody>(static_cast<entt::entity>(entity))->body);
		t = std::move(phys_body->getGlobalPose());
		t.q *= physx::PxGetRotZQuat(physx::PxDegToRad(rotz)) * physx::PxGetRotYQuat(physx::PxDegToRad(roty)) * physx::PxGetRotXQuat(physx::PxDegToRad(rotz));
		phys_body->setGlobalPose(t);
	}
	else {
		t = std::move(physx::PxTransform(physx::PxMat44(transform.matrix)));
		t.q *= physx::PxGetRotZQuat(physx::PxDegToRad(rotz)) * physx::PxGetRotYQuat(physx::PxDegToRad(roty)) * physx::PxGetRotXQuat(physx::PxDegToRad(rotz));
	}
	CopyMatrix(transform.matrix, physx::PxMat44(t).front());
}

// Set an object's rotation.
void WorldSystem_EnTT::SetRotation(GameObject entity, float rotx, float roty, float rotz) {
	auto& transform = scenes_[current_scene_].get<Transform>(static_cast<entt::entity>(entity));
	PhysicsBody* body_component = scenes_[current_scene_].try_get<PhysicsBody>(static_cast<entt::entity>(entity));
	physx::PxTransform t;
	if (body_component) {
		physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(scenes_[current_scene_].try_get<PhysicsBody>(static_cast<entt::entity>(entity))->body);
		t.q = physx::PxGetRotZQuat(physx::PxDegToRad(rotz)) * physx::PxGetRotYQuat(physx::PxDegToRad(roty)) * physx::PxGetRotXQuat(physx::PxDegToRad(rotz));
		phys_body->setGlobalPose(t);
	}
	else {
		t = std::move(physx::PxTransform(physx::PxMat44(transform.matrix)));
		t.q = physx::PxGetRotZQuat(physx::PxDegToRad(rotz)) * physx::PxGetRotYQuat(physx::PxDegToRad(roty)) * physx::PxGetRotXQuat(physx::PxDegToRad(rotz));
	}
	CopyMatrix(transform.matrix, physx::PxMat44(t).front());
}

// Set an object's position.
void WorldSystem_EnTT::SetPosition(GameObject entity, float x, float y, float z) {
	auto& transform = scenes_[current_scene_].get<Transform>(static_cast<entt::entity>(entity));
	transform.matrix[12] = x;
	transform.matrix[13] = y;
	transform.matrix[14] = z;
	PhysicsBody* body_component = scenes_[current_scene_].try_get<PhysicsBody>(static_cast<entt::entity>(entity));
	if (body_component) {
		physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(scenes_[current_scene_].try_get<PhysicsBody>(static_cast<entt::entity>(entity))->body);
		physx::PxTransform t = phys_body->getGlobalPose();
		t.p = physx::PxVec3(x, y, z);
		phys_body->setGlobalPose(t);
	}
}

// Set a light source's direction.
void Toffee::Internal::WorldSystem_EnTT::SetDirection(GameObject entity, float x, float y, float z) {
	Light* light = scenes_[current_scene_].try_get<Light>(static_cast<entt::entity>(entity));
	if (light == nullptr) {
		printf("Cannot set direction on entity which isn't a light source\n");
		return;
	}
	if (!light->has_position) SetPosition(entity, -x, -y, -z);
	else {
		light->spot_dir[0] = x;
		light->spot_dir[1] = y;
		light->spot_dir[2] = z;
	}
}

// Set an object's scale.
void WorldSystem_EnTT::SetScale(GameObject entity, float scale_x, float scale_y, float scale_z) {
	PhysicsBody* body_component = scenes_[current_scene_].try_get<PhysicsBody>(static_cast<entt::entity>(entity));

	if (!body_component) { // This object isn't a physics object so just update the transform directly
		auto& transform = scenes_[current_scene_].get<Transform>(static_cast<entt::entity>(entity));
		transform.scale_x = scale_x;
		transform.scale_y = scale_y;
		transform.scale_z = scale_z;
	}
	else {
		physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(body_component->body);
		physx::PxShape* shape;
		phys_body->getShapes(&shape, 1);

		physx::PxGeometryHolder geometry(shape->getGeometry()); //How to do this stuff: https://nvidia-omniverse.github.io/PhysX/physx/5.3.0/docs/Geometry.html#pxgeometryholder
		if (geometry.getType() == physx::PxGeometryType::eBOX) {
			auto& transform = scenes_[current_scene_].get<Transform>(static_cast<entt::entity>(entity));
			transform.scale_x = std::move(scale_x);
			transform.scale_y = std::move(scale_y);
			transform.scale_z = std::move(scale_z);
			physx::PxVec3 extents = std::move(geometry.box().halfExtents);
			shape->setGeometry(physx::PxBoxGeometry(scale_x * extents.x, scale_y * extents.y, scale_z * extents.z));
		}
		else printf("XYZ scaling is only for box bodies or unsimulated objects. Spheres scale only radius, capsules scale only radius and height\n");
	}
}

// Set scale of object with a capsule physics body
void WorldSystem_EnTT::SetScale(GameObject entity, float scale_radius, float scale_height) {
	PhysicsBody* body_component = scenes_[current_scene_].try_get<PhysicsBody>(static_cast<entt::entity>(entity));
	if (!body_component) {
		printf("Unsimulated objects require scaling all 3 axis\n");
	}
	else {
		physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(body_component->body);
		physx::PxShape* shape;
		phys_body->getShapes(&shape, 1);

		physx::PxGeometryHolder geometry(shape->getGeometry()); //How to do this stuff: https://nvidia-omniverse.github.io/PhysX/physx/5.3.0/docs/Geometry.html#pxgeometryholder
		if (geometry.getType() == physx::PxGeometryType::eCAPSULE) {
			auto& transform = scenes_[current_scene_].get<Transform>(static_cast<entt::entity>(entity));
			transform.scale_x = scale_radius;
			transform.scale_y = scale_height;
			transform.scale_z = scale_radius;
			float rad = geometry.capsule().radius;
			float height = geometry.capsule().halfHeight;
			shape->setGeometry(physx::PxCapsuleGeometry(rad * scale_radius, height * scale_height));
		}
		else printf("Radius and height scaling is only for capsule bodies\n");
	}
}

// Set scale of object with a sphere physics body
void WorldSystem_EnTT::SetScale(GameObject entity, float scale_radius) {
	PhysicsBody* body_component = scenes_[current_scene_].try_get<PhysicsBody>(static_cast<entt::entity>(entity));
	if (!body_component) {
		printf("Unsimulated objects require scaling all 3 axis\n");
	}
	else {
		physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(body_component->body);
		physx::PxShape* shape;
		phys_body->getShapes(&shape, 1);

		physx::PxGeometryHolder geometry(shape->getGeometry()); //How to do this stuff: https://nvidia-omniverse.github.io/PhysX/physx/5.3.0/docs/Geometry.html#pxgeometryholder
		if (geometry.getType() == physx::PxGeometryType::eCAPSULE) {
			auto& transform = scenes_[current_scene_].get<Transform>(static_cast<entt::entity>(entity));
			transform.scale_x = scale_radius;
			transform.scale_y = scale_radius;
			transform.scale_z = scale_radius;
			float rad = geometry.capsule().radius;
			float height = geometry.capsule().halfHeight;
			shape->setGeometry(physx::PxCapsuleGeometry(rad * scale_radius));
		}
		else printf("Radius and height scaling is only for capsule bodies\n");
	}
}

// Set an object's mesh.
void WorldSystem_EnTT::SetMesh(GameObject entity, std::string name) {
	auto& renderable = scenes_[current_scene_].get_or_emplace<Renderable>(static_cast<entt::entity>(entity));
	if (name == "Quad" || name == "Plane" || name == "Cube" || name == "Sphere") renderable.triangles = false;
	renderable.mesh = reinterpret_cast<Mesh*>(ToffeeAPI::Resources()->GetResource(name));
}

// Set an object's texture.
void WorldSystem_EnTT::SetTexture(GameObject entity, std::string name) {
	Renderable* renderable = scenes_[current_scene_].try_get<Renderable>(static_cast<entt::entity>(entity));
	if (!renderable) {
		printf("Cannot set texture on object with no mesh.");
		return;
	}
	renderable->texture = ToffeeAPI::Resources()->GetResource(name);
}

// Set the ambient component of an object's material.
void WorldSystem_EnTT::SetAmbientMat(GameObject entity, float r, float g, float b, float a) {
	Renderable* renderable = scenes_[current_scene_].try_get<Renderable>(static_cast<entt::entity>(entity));
	if (!renderable) {
		printf("Cannot set material property on object with no mesh.");
		return;
	}
	renderable->ambient[0] = r;
	renderable->ambient[1] = g;
	renderable->ambient[2] = b;
	renderable->ambient[3] = a;
}

// Set the diffuse component of an object's material.
void WorldSystem_EnTT::SetDiffuseMat(GameObject entity, float r, float g, float b, float a) {
	Renderable* renderable = scenes_[current_scene_].try_get<Renderable>(static_cast<entt::entity>(entity));
	if (!renderable) {
		printf("Cannot set material property on object with no mesh.");
		return;
	}
	renderable->diffuse[0] = r;
	renderable->diffuse[1] = g;
	renderable->diffuse[2] = b;
	renderable->diffuse[3] = a;
}

// Set the ambient component of a light source.
void WorldSystem_EnTT::SetAmbientSource(GameObject entity, float r, float g, float b, float a) {
	Light* light = scenes_[current_scene_].try_get<Light>(static_cast<entt::entity>(entity));
	if (!light) {
		int num_lights = scenes_[current_scene_].view<Light>().size();
		if (num_lights >= 8) {
			printf("Max lights reached. Cannot create any more lights in scene\n");
			return;
		}
		scenes_[current_scene_].emplace<Light>(static_cast<entt::entity>(entity));
		light = &scenes_[current_scene_].get<Light>(static_cast<entt::entity>(entity));
	}
	light->ambient[0] = r;
	light->ambient[1] = g;
	light->ambient[2] = b;
	light->ambient[3] = a;
}

// Set the diffuse component of a light source.
void WorldSystem_EnTT::SetDiffuseSource(GameObject entity, float r, float g, float b, float a) {
	Light* light = scenes_[current_scene_].try_get<Light>(static_cast<entt::entity>(entity));
	if (!light) {
		int num_lights = scenes_[current_scene_].view<Light>().size();
		if (num_lights >= 8) {
			printf("Max lights reached. Cannot create any more lights in scene\n");
			return;
		}
		scenes_[current_scene_].emplace<Light>(static_cast<entt::entity>(entity));
		light = &scenes_[current_scene_].get<Light>(static_cast<entt::entity>(entity));
	}
	light->diffuse[0] = r;
	light->diffuse[1] = g;
	light->diffuse[2] = b;
	light->diffuse[3] = a;
}

// Create a new scene
void Toffee::Internal::WorldSystem_EnTT::NewScene(std::string name) {
	scenes_[name] = std::move(entt::registry());
}

// Switch to a specified scene (create it if it doesn't exist yet).
void Toffee::Internal::WorldSystem_EnTT::SwitchScene(std::string name) {
	if (!scenes_.count(name)) NewScene(name);
	current_scene_ = name;
	SwitchPhysicsScene(name);
}

// Destroy a scene and all objects in it.
void Toffee::Internal::WorldSystem_EnTT::DestroyScene(std::string name) {
	scenes_[name].clear();
	scenes_.erase(scenes_.find(name));
}

// Update transform components to match values from the physics simulation
void Toffee::Internal::WorldSystem_EnTT::Update() {
	auto simulated_list = scenes_[current_scene_].view<Transform, PhysicsBody>();
	
	for (auto& object : simulated_list) {
		auto& transform = scenes_[current_scene_].get<Transform>(static_cast<entt::entity>(object));
		auto px_body = reinterpret_cast<physx::PxRigidActor*>(simulated_list.get<PhysicsBody>(object).body);

		auto px_pose = physx::PxMat44(px_body->getGlobalPose()).front();
		
		CopyMatrix(transform.matrix, px_pose);
	}
}

// Copy matrix 2 to matrix 1
void Toffee::Internal::WorldSystem_EnTT::CopyMatrix(float* mat_1, const float* mat_2) {
	mat_1[0] = mat_2[0];
	mat_1[1] = mat_2[1];
	mat_1[2] = mat_2[2];
	mat_1[3] = mat_2[3];
	mat_1[4] = mat_2[4];
	mat_1[5] = mat_2[5];
	mat_1[6] = mat_2[6];
	mat_1[7] = mat_2[7];
	mat_1[8] = mat_2[8];
	mat_1[9] = mat_2[9];
	mat_1[10] = mat_2[10];
	mat_1[11] = mat_2[11];
	mat_1[12] = mat_2[12];
	mat_1[13] = mat_2[13];
	mat_1[14] = mat_2[14];
	mat_1[15] = mat_2[15];
}

