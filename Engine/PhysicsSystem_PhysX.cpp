#include "pch.h"
#include "PhysicsSystem_PhysX.h"
#include "SystemsLocator.h"
#include <entt/entity/registry.hpp>

using namespace Toffee::Internal;

PhysicsSystem_PhysX* physics_px = new PhysicsSystem_PhysX;

PhysicsSystem_PhysX::PhysicsSystem_PhysX() {
	ProvideSystem(this);
	physics_px = nullptr;
}

int PhysicsSystem_PhysX::Initialise() {
	using namespace physx;
	px_foundation_ = PxCreateFoundation(PX_PHYSICS_VERSION, px_allocator_, px_error_callback_);

	px_physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *px_foundation_, PxTolerancesScale(), true);

	px_scene_desc_ = new PxSceneDesc(px_physics_->getTolerancesScale());
	px_scene_desc_->gravity = PxVec3(0.0f, -9.81f, 0.0f);
	px_dispatcher_ = PxDefaultCpuDispatcherCreate(2);
	px_scene_desc_->cpuDispatcher = px_dispatcher_;
	px_scene_desc_->filterShader = PxDefaultSimulationFilterShader;

	return true;
}

// Step forward the physx simulation
bool Toffee::Internal::PhysicsSystem_PhysX::Update(float dt) {
	if (simulating_) {
		accumulator_ += dt;
		if (accumulator_ < step_size_) { // If enough time hasn't passed, do nothing 
			return false;
		}
		do { // Step forward until simulation has caught up to real-time
			accumulator_ -= step_size_;
			current_px_scene_->simulate(step_size_);
			current_px_scene_->fetchResults(true);
		} while (accumulator_ >= step_size_);

		return true;
	}
}

int PhysicsSystem_PhysX::Shutdown()
{
	px_physics_->release();
	px_foundation_->release();

	return 0;
}

// Set global gravity.
void PhysicsSystem_PhysX::SetGravity(float x, float y, float z) {
	current_px_scene_->setGravity(physx::PxVec3(x, y, z));
}

// Activate/deactivate simulation of an object.
void PhysicsSystem_PhysX::SetActive(GameObject entity, bool value) {
	auto phys_component = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList())->try_get<Toffee::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, value);
	}
}

// Apply an impulse to an object.
void PhysicsSystem_PhysX::ApplyImpulse(GameObject entity, float x, float y, float z) {
	auto phys_component = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList())->try_get<Toffee::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body)->addForce(physx::PxVec3(x, y, z), physx::PxForceMode::eIMPULSE);
	}
}

// Add a vector to an object's velocity.
void PhysicsSystem_PhysX::ApplyVelocity(GameObject entity, float x, float y, float z) {
	auto phys_component = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList())->try_get<Toffee::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		physx::PxRigidDynamic* phys_body = reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body);
		physx::PxVec3 vel = phys_body->getLinearVelocity();
		vel += physx::PxVec3(x, y, z);
		phys_body->setLinearVelocity(vel);
	}
}

// Add a vector to an object's angular velocity.
void PhysicsSystem_PhysX::ApplyAngularVelocity(GameObject entity, float x, float y, float z) {
	auto phys_component = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList())->try_get<Toffee::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		physx::PxRigidDynamic* phys_body = reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body);
		physx::PxVec3 vel = phys_body->getAngularVelocity();
		vel += physx::PxVec3(x, y, z);
		phys_body->setAngularVelocity(vel);
	}
}

// Set an object's velocity.
void PhysicsSystem_PhysX::SetVelocity(GameObject entity, float x, float y, float z) {
	auto phys_component = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList())->try_get<Toffee::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body)->setLinearVelocity(physx::PxVec3(x, y, z));
	}
}

// Set the angular velocity of an object.
void PhysicsSystem_PhysX::SetAngularVelocity(GameObject entity, float x, float y, float z) {
	auto phys_component = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList())->try_get<Toffee::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body)->setAngularVelocity(physx::PxVec3(x, y, z));
	}
}

// Set angular damping of an object.
void PhysicsSystem_PhysX::SetAngularDamping(GameObject entity, float value) {
	auto phys_component = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList())->try_get<Toffee::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body)->setAngularDamping(value);
	}
}

// Switch active scene.
void Toffee::Internal::PhysicsSystem_PhysX::SwitchScene(std::string scene) {
	if (!px_scenes_.count(scene)) px_scenes_[scene] = px_physics_->createScene(*px_scene_desc_);
	current_px_scene_ = px_scenes_[scene];
}

// Add a box shaped physics body to a game object.
void PhysicsSystem_PhysX::AddBoxBody(Toffee::GameObject entity, float half_x, float half_y, float half_z, PhysicsType type, float friction, float restitution) {
	auto world_scene = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList());
	auto& phys_body = world_scene->get_or_emplace<Toffee::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	auto& transform = world_scene->get<Toffee::Components::Transform>(static_cast<entt::entity>(entity));
	physx::PxMaterial* gMaterial = px_physics_->createMaterial(friction, friction, restitution);
	physx::PxShape* shape = px_physics_->createShape(physx::PxBoxGeometry(half_x, half_y, half_z), *gMaterial);
	gMaterial->release();

	phys_body.body = AddBody(shape, type, transform);
}

// Add a sphere shaped physics body to a game object.
void PhysicsSystem_PhysX::AddSphereBody(Toffee::GameObject entity, float radius, PhysicsType type, float friction, float restitution) {
	auto world_scene = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList());
	auto& phys_body = world_scene->get_or_emplace<Toffee::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	auto& transform = world_scene->get<Toffee::Components::Transform>(static_cast<entt::entity>(entity));
	physx::PxMaterial* gMaterial = px_physics_->createMaterial(friction, friction, restitution);
	physx::PxShape* shape = px_physics_->createShape(physx::PxSphereGeometry(radius), *gMaterial);
	gMaterial->release();

	phys_body.body = AddBody(shape, type, transform);
}

// Add a capsule shaped physics body to a game object.
void PhysicsSystem_PhysX::AddCapsuleBody(Toffee::GameObject entity, float half_extent, float radius, PhysicsType type, float friction, float restitution) {
	auto world_scene = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList());
	auto& phys_body = world_scene->get_or_emplace<Toffee::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	auto& transform = world_scene->get<Toffee::Components::Transform>(static_cast<entt::entity>(entity));
	physx::PxMaterial* gMaterial = px_physics_->createMaterial(friction, friction, restitution);
	physx::PxShape* shape = px_physics_->createShape(physx::PxCapsuleGeometry(radius, half_extent), *gMaterial);

	// Need to adjust the local pose so it stands upright
	physx::PxTransform relative_pose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	shape->setLocalPose(relative_pose);
	gMaterial->release();

	phys_body.body = AddBody(shape, type, transform);
}

// Add an infinite, horizontal static plane shaped physics body to a game object.
void PhysicsSystem_PhysX::AddPlaneBody(Toffee::GameObject entity) {
	auto world_scene = reinterpret_cast<entt::registry*>(ToffeeAPI::World()->GetObjectsList());
	auto& phys_body = world_scene->get_or_emplace<Toffee::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	auto& transform = world_scene->get<Toffee::Components::Transform>(static_cast<entt::entity>(entity));
	physx::PxMaterial* gMaterial = px_physics_->createMaterial(1, 1, 1);
	physx::PxShape* shape = px_physics_->createShape(physx::PxPlaneGeometry(), *gMaterial);

	// Need to adjust the local pose so it's horizontal facing upwards
	physx::PxTransform relative_pose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	shape->setLocalPose(relative_pose);
	gMaterial->release();

	phys_body.body = AddBody(shape, PhysicsType::STATIC, transform);
}

// Add a body to the PhysX scene.
physx::PxRigidActor* PhysicsSystem_PhysX::AddBody(physx::PxShape* shape, PhysicsType type, Toffee::Components::Transform& transform) {
	physx::PxTransform position(physx::PxMat44(transform.matrix));
	physx::PxRigidActor* px_body = nullptr;

	if (type == PhysicsType::DYNAMIC) {
		px_body = px_physics_->createRigidDynamic(position);
		px_body->attachShape(*shape);
		physx::PxRigidBodyExt::updateMassAndInertia(*px_body->is<physx::PxRigidDynamic>(), 1.f);
		px_body->is<physx::PxRigidDynamic>()->setSolverIterationCounts(8, 3);
	}
	else {
		px_body = px_physics_->createRigidStatic(position);
		px_body->attachShape(*shape);
	}
	current_px_scene_->addActor(*px_body);

	shape->release();

	return px_body;
}



