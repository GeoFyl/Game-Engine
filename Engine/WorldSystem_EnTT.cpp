#include "pch.h"
#include "WorldSystem_EnTT.h"
#include "SystemsLocator.h"
#include "PhysicsSystem_PhysX.h"

using namespace Engine::Internal;
using namespace Engine::Components;

WorldSystem_EnTT* world_entt = new WorldSystem_EnTT;

WorldSystem_EnTT::WorldSystem_EnTT() {
	ProvideSystem(this);
	world_entt = nullptr;
}

int WorldSystem_EnTT::Initialise()
{
	return 0;
}

int WorldSystem_EnTT::Shutdown()
{
	return 0;
}

Engine::GameObject WorldSystem_EnTT::CreateGameObject()
{
	return CreateGameObject(0, 0, 0);
}

Engine::GameObject WorldSystem_EnTT::CreateGameObject(float x, float y, float z)
{
	auto entity = registry_.create();
	auto& transform = registry_.emplace<Transform>(entity);
	CopyMatrix(transform.matrix, physx::PxMat44(physx::PxMat33(physx::PxIdentity), physx::PxVec3(x, y, z)).front());
	return (GameObject)entity;
}

Engine::GameObject WorldSystem_EnTT::CreateDirectionalLight() {
	return CreateDirectionalLight(-1, 0, 0);
}

Engine::GameObject WorldSystem_EnTT::CreateDirectionalLight(float x, float y, float z) {
	if (num_lights >= 8) printf("Max lights reached. Cannot create any more lights\n");
	num_lights++;
	auto entity = registry_.create();
	auto& transform = registry_.emplace<Transform>(entity);
	CopyMatrix(transform.matrix, physx::PxMat44(physx::PxMat33(physx::PxIdentity), physx::PxVec3(-x, -y, -z)).front());
	auto& light = registry_.emplace<Light>(entity);
	light.has_position = false;
	return (GameObject)entity;
}

Engine::GameObject WorldSystem_EnTT::CreatePositionalLight() {
	return CreatePositionalLight(0, 0, 0);
}

Engine::GameObject WorldSystem_EnTT::CreatePositionalLight(float x, float y, float z) {
	if (num_lights >= 8) printf("Max lights reached. Cannot create any more lights\n");
	num_lights++;
	auto entity = registry_.create();
	auto& transform = registry_.emplace<Transform>(entity);
	CopyMatrix(transform.matrix, physx::PxMat44(physx::PxMat33(physx::PxIdentity), physx::PxVec3(x, y, z)).front());
	registry_.emplace<Light>(entity);
	return (GameObject)entity;
}

Engine::GameObject WorldSystem_EnTT::CreateSpotLight(float x, float y, float z, float d_x, float d_y, float d_z) {
	if (num_lights >= 8) printf("Max lights reached. Cannot create any more lights\n");
	num_lights++;
	auto entity = registry_.create();
	auto& transform = registry_.emplace<Transform>(entity);
	CopyMatrix(transform.matrix, physx::PxMat44(physx::PxMat33(physx::PxIdentity), physx::PxVec3(x, y, z)).front());
	Light& light = registry_.emplace<Light>(entity);
	light.spot_dir[0] = d_x;
	light.spot_dir[1] = d_y;
	light.spot_dir[2] = d_z;
	light.spot_cutoff = 50.f;
	light.spot_exp = 20.f;
	return (GameObject)entity;
}


Engine::GameObject WorldSystem_EnTT::CreateSpotLight() {
	return CreateSpotLight(0, 0, 0, 0, -1, 0);
}

void WorldSystem_EnTT::Move(GameObject entity, float x, float y, float z) {
	auto& transform = registry_.get<Transform>(static_cast<entt::entity>(entity));
	transform.matrix[12] += x;
	transform.matrix[13] += y;
	transform.matrix[14] += z;
	physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(registry_.try_get<PhysicsBody>(static_cast<entt::entity>(entity))->body);
	if (phys_body) {
		physx::PxTransform t = phys_body->getGlobalPose();
		t.p += physx::PxVec3(x, y, z);
		phys_body->setGlobalPose(t);
	}
}

void WorldSystem_EnTT::Rotate(GameObject entity, float rotx, float roty, float rotz) {
	auto& transform = registry_.get<Transform>(static_cast<entt::entity>(entity));
	physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(registry_.try_get<PhysicsBody>(static_cast<entt::entity>(entity))->body);
	physx::PxTransform t;
	if (phys_body) {
		t = std::move(phys_body->getGlobalPose());
		t.q *= physx::PxGetRotZQuat(physx::PxDegToRad(rotz)) * physx::PxGetRotYQuat(physx::PxDegToRad(roty)) * physx::PxGetRotXQuat(physx::PxDegToRad(rotz));
		phys_body->setGlobalPose(t);
	}
	else {
		t = std::move(physx::PxTransform(physx::PxMat44(transform.matrix)));
		t.q *= physx::PxGetRotZQuat(physx::PxDegToRad(rotz)) * physx::PxGetRotYQuat(physx::PxDegToRad(roty)) * physx::PxGetRotXQuat(physx::PxDegToRad(rotz));
		CopyMatrix(transform.matrix, physx::PxMat44(t).front());
	}
}

void WorldSystem_EnTT::SetRotation(GameObject entity, float rotx, float roty, float rotz) {
	auto& transform = registry_.get<Transform>(static_cast<entt::entity>(entity));
	physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(registry_.try_get<PhysicsBody>(static_cast<entt::entity>(entity))->body);
	if (phys_body) {
		physx::PxTransform t = phys_body->getGlobalPose();
		t.q = physx::PxGetRotZQuat(physx::PxDegToRad(rotz)) * physx::PxGetRotYQuat(physx::PxDegToRad(roty)) * physx::PxGetRotXQuat(physx::PxDegToRad(rotz));
		phys_body->setGlobalPose(t);
	}
	else {
		physx::PxTransform t(physx::PxMat44(transform.matrix));
		t.q = physx::PxGetRotZQuat(physx::PxDegToRad(rotz)) * physx::PxGetRotYQuat(physx::PxDegToRad(roty)) * physx::PxGetRotXQuat(physx::PxDegToRad(rotz));
		CopyMatrix(transform.matrix, physx::PxMat44(t).front());
	}
}

void WorldSystem_EnTT::SetPosition(GameObject entity, float x, float y, float z) {
	auto& transform = registry_.get<Transform>(static_cast<entt::entity>(entity));
	transform.matrix[12] = x;
	transform.matrix[13] = y;
	transform.matrix[14] = z;
	physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(registry_.try_get<PhysicsBody>(static_cast<entt::entity>(entity)));
	if (phys_body) {
		physx::PxTransform t = phys_body->getGlobalPose();
		t.p = physx::PxVec3(x, y, z);
		phys_body->setGlobalPose(t);
	}
}

void Engine::Internal::WorldSystem_EnTT::SetDirection(GameObject entity, float x, float y, float z) {
	Light* light = registry_.try_get<Light>(static_cast<entt::entity>(entity));
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

void WorldSystem_EnTT::SetScale(GameObject entity, float scale_x, float scale_y, float scale_z) {
	PhysicsBody* body_component = registry_.try_get<PhysicsBody>(static_cast<entt::entity>(entity));

	if (!body_component) {
		auto& transform = registry_.get<Transform>(static_cast<entt::entity>(entity));
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
			auto& transform = registry_.get<Transform>(static_cast<entt::entity>(entity));
			transform.scale_x = std::move(scale_x);
			transform.scale_y = std::move(scale_y);
			transform.scale_z = std::move(scale_z);
			physx::PxVec3 extents = std::move(geometry.box().halfExtents);
			shape->setGeometry(physx::PxBoxGeometry(scale_x * extents.x, scale_y * extents.y, scale_z * extents.z));
		}
		else printf("XYZ scaling is only for box bodies or unsimulated objects. Spheres scale only radius, capsules scale only radius and height\n");
	}
}

void WorldSystem_EnTT::SetScale(GameObject entity, float scale_radius, float scale_height) {
	PhysicsBody* body_component = registry_.try_get<PhysicsBody>(static_cast<entt::entity>(entity));
	if (!body_component) {
		printf("Unsimulated objects require scaling all 3 axis\n");
	}
	else {
		physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(body_component->body);
		physx::PxShape* shape;
		phys_body->getShapes(&shape, 1);

		physx::PxGeometryHolder geometry(shape->getGeometry()); //How to do this stuff: https://nvidia-omniverse.github.io/PhysX/physx/5.3.0/docs/Geometry.html#pxgeometryholder
		if (geometry.getType() == physx::PxGeometryType::eCAPSULE) {
			auto& transform = registry_.get<Transform>(static_cast<entt::entity>(entity));
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

void WorldSystem_EnTT::SetScale(GameObject entity, float scale_radius) {
	PhysicsBody* body_component = registry_.try_get<PhysicsBody>(static_cast<entt::entity>(entity));
	if (!body_component) {
		printf("Unsimulated objects require scaling all 3 axis\n");
	}
	else {
		physx::PxRigidActor* phys_body = reinterpret_cast<physx::PxRigidActor*>(body_component->body);
		physx::PxShape* shape;
		phys_body->getShapes(&shape, 1);

		physx::PxGeometryHolder geometry(shape->getGeometry()); //How to do this stuff: https://nvidia-omniverse.github.io/PhysX/physx/5.3.0/docs/Geometry.html#pxgeometryholder
		if (geometry.getType() == physx::PxGeometryType::eCAPSULE) {
			auto& transform = registry_.get<Transform>(static_cast<entt::entity>(entity));
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

void WorldSystem_EnTT::SetMesh(GameObject entity, std::string name) {
	auto& renderable = registry_.get_or_emplace<Renderable>(static_cast<entt::entity>(entity));
	if (name == "Quad" || name == "Plane" || name == "Cube" || name == "Sphere") renderable.triangles = false;
	renderable.mesh = reinterpret_cast<Mesh*>(SystemsAPI::Resources()->GetResource(name));
}

void WorldSystem_EnTT::SetTexture(GameObject entity, std::string name) {
	Renderable* renderable = registry_.try_get<Renderable>(static_cast<entt::entity>(entity));
	if (!renderable) {
		printf("Cannot set texture on object with no mesh.");
		return;
	}
	renderable->texture = SystemsAPI::Resources()->GetResource(name);
}

void WorldSystem_EnTT::SetAmbientMat(GameObject entity, float r, float g, float b, float a) {
	Renderable* renderable = registry_.try_get<Renderable>(static_cast<entt::entity>(entity));
	if (!renderable) {
		printf("Cannot set material property on object with no mesh.");
		return;
	}
	renderable->ambient[0] = r;
	renderable->ambient[1] = g;
	renderable->ambient[2] = b;
	renderable->ambient[3] = a;
}

void WorldSystem_EnTT::SetDiffuseMat(GameObject entity, float r, float g, float b, float a) {
	Renderable* renderable = registry_.try_get<Renderable>(static_cast<entt::entity>(entity));
	if (!renderable) {
		printf("Cannot set material property on object with no mesh.");
		return;
	}
	renderable->diffuse[0] = r;
	renderable->diffuse[1] = g;
	renderable->diffuse[2] = b;
	renderable->diffuse[3] = a;
}

void WorldSystem_EnTT::SetAmbientSource(GameObject entity, float r, float g, float b, float a) {
	Light* light = registry_.try_get<Light>(static_cast<entt::entity>(entity));
	if (!light) {
		if (num_lights >= 8) {
			printf("Max lights reached. Cannot set ambient light source\n");
			return;
		}
		registry_.emplace<Light>(static_cast<entt::entity>(entity));
		light = &registry_.get<Light>(static_cast<entt::entity>(entity));
	}
	light->ambient[0] = r;
	light->ambient[1] = g;
	light->ambient[2] = b;
	light->ambient[3] = a;
}

void WorldSystem_EnTT::SetDiffuseSource(GameObject entity, float r, float g, float b, float a) {
	Light* light = registry_.try_get<Light>(static_cast<entt::entity>(entity));
	if (light == nullptr) {
		if (num_lights >= 8) {
			printf("Max lights reached. Cannot set diffuse light source\n");
			return;
		}
		registry_.emplace<Light>(static_cast<entt::entity>(entity));
		light = &registry_.get<Light>(static_cast<entt::entity>(entity));
	}
	light->diffuse[0] = r;
	light->diffuse[1] = g;
	light->diffuse[2] = b;
	light->diffuse[3] = a;
}

Transform WorldSystem_EnTT::GetPosition(GameObject entity) {
	return registry_.get<Transform>(static_cast<entt::entity>(entity));
}

void Engine::Internal::WorldSystem_EnTT::Update() {
	auto simulated_list = registry_.view<Transform, PhysicsBody>();
	
	for (auto& object : simulated_list) {
		auto& transform = registry_.get<Transform>(static_cast<entt::entity>(object));
		auto px_body = reinterpret_cast<physx::PxRigidActor*>(simulated_list.get<PhysicsBody>(object).body);

		auto px_pose = physx::PxMat44(px_body->getGlobalPose()).front();
		
		CopyMatrix(transform.matrix, px_pose);
		//SetPosition(static_cast<GameObject>(object), px_pose.p.x, px_pose.p.y, px_pose.p.z);

		//float rot_x = (px_pose.q.x >= 0 ? 1 : -1) * px_pose.q.getAngle(physx::PxQuat(0, px_pose.q.getBasisVector0())) / 0.01745329251994329547f;
		//float rot_y = (px_pose.q.y >= 0 ? 1 : -1) * px_pose.q.getAngle(physx::PxQuat(0, px_pose.q.getBasisVector1())) / 0.01745329251994329547f; //make these members
		//float rot_z = (px_pose.q.z >= 0 ? 1 : -1) * px_pose.q.getAngle(physx::PxQuat(0, px_pose.q.getBasisVector2())) / 0.01745329251994329547f;

		//float rot_x = px_pose.q.getBasisVector0(). / 0.01745329251994329547f;
		//float rot_y = (px_pose.q.y >= 0 ? 1 : -1) * px_pose.q.getAngle(physx::PxGetRotYQuat(0)) / 0.01745329251994329547f; //make these members
		//float rot_z = (px_pose.q.z >= 0 ? 1 : -1) * px_pose.q.getAngle(physx::PxQuat(0, px_pose.q.getBasisVector2())) / 0.01745329251994329547f;


		
		//printf("z: %f, q.y: %f\n", rot_y, px_pose.q.y);

		//SetRotation(static_cast<GameObject>(object), 0, rot_y, 0);
	}
}

void Engine::Internal::WorldSystem_EnTT::CopyMatrix(float* mat_1, const float* mat_2) {
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

