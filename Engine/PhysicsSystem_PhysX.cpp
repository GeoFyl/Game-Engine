#include "pch.h"
#include "PhysicsSystem_PhysX.h"
#include "SystemsLocator.h"
#include <entt/entity/registry.hpp>

using namespace Engine::Internal;

PhysicsSystem_PhysX* physics_px = new PhysicsSystem_PhysX;

PhysicsSystem_PhysX::PhysicsSystem_PhysX() {
	ProvideSystem(this);
	physics_px = nullptr;
}

void PhysicsSystem_PhysX::SetGravity(float x, float y, float z) {
	px_scene_->setGravity(physx::PxVec3(x, y, z));
}

void PhysicsSystem_PhysX::SetActive(GameObject entity, bool value) {
	auto phys_component = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList())->try_get<Engine::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, value);
	}
}

void PhysicsSystem_PhysX::ApplyImpulse(GameObject entity, float x, float y, float z) {
	auto phys_component = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList())->try_get<Engine::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body)->addForce(physx::PxVec3(x, y, z), physx::PxForceMode::eIMPULSE);
	}
}

void PhysicsSystem_PhysX::ApplyVelocity(GameObject entity, float x, float y, float z) {
	auto phys_component = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList())->try_get<Engine::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		physx::PxRigidDynamic* phys_body = reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body);
		physx::PxVec3 vel = phys_body->getLinearVelocity();
		vel += physx::PxVec3(x, y, z);
		phys_body->setLinearVelocity(vel);
	}
}

void PhysicsSystem_PhysX::ApplyAngularVelocity(GameObject entity, float x, float y, float z) {
	auto phys_component = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList())->try_get<Engine::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		physx::PxRigidDynamic* phys_body = reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body);
		physx::PxVec3 vel = phys_body->getAngularVelocity();
		vel += physx::PxVec3(x, y, z);
		phys_body->setAngularVelocity(vel);
	}
}

void PhysicsSystem_PhysX::SetVelocity(GameObject entity, float x, float y, float z) {
	auto phys_component = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList())->try_get<Engine::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body)->setLinearVelocity(physx::PxVec3(x, y, z));
	}
}

void PhysicsSystem_PhysX::SetAngularVelocity(GameObject entity, float x, float y, float z) {
	auto phys_component = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList())->try_get<Engine::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body)->setAngularVelocity(physx::PxVec3(x, y, z));
	}
}

void PhysicsSystem_PhysX::SetAngularDamping(GameObject entity, float value) {
	auto phys_component = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList())->try_get<Engine::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	if (phys_component) {
		reinterpret_cast<physx::PxRigidDynamic*>(phys_component->body)->setAngularDamping(value);
	}
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
	px_scene_ = px_physics_->createScene(*px_scene_desc_);

	return true;
}

int PhysicsSystem_PhysX::Shutdown()
{
	px_physics_->release();
	px_foundation_->release();

	return 0;
}

void PhysicsSystem_PhysX::AddBoxBody(Engine::GameObject entity, float half_x, float half_y, float half_z, PhysicsType type, float friction, float restitution) {
	auto world_scene = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList());
	auto& phys_body = world_scene->get_or_emplace<Engine::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	auto& transform = world_scene->get<Engine::Components::Transform>(static_cast<entt::entity>(entity));
	physx::PxMaterial* gMaterial = px_physics_->createMaterial(friction, friction, restitution);
	physx::PxShape* shape = px_physics_->createShape(physx::PxBoxGeometry(half_x, half_y, half_z), *gMaterial);
	gMaterial->release();

	phys_body.body = AddBody(shape, type, transform);
}

void PhysicsSystem_PhysX::AddSphereBody(Engine::GameObject entity, float radius, PhysicsType type, float friction, float restitution) {
	auto world_scene = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList());
	auto& phys_body = world_scene->get_or_emplace<Engine::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	auto& transform = world_scene->get<Engine::Components::Transform>(static_cast<entt::entity>(entity));
	physx::PxMaterial* gMaterial = px_physics_->createMaterial(friction, friction, restitution);
	physx::PxShape* shape = px_physics_->createShape(physx::PxSphereGeometry(radius), *gMaterial);
	gMaterial->release();

	phys_body.body = AddBody(shape, type, transform);
}

void PhysicsSystem_PhysX::AddCapsuleBody(Engine::GameObject entity, float half_extent, float radius, PhysicsType type, float friction, float restitution) {
	auto world_scene = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList());
	auto& phys_body = world_scene->get_or_emplace<Engine::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	auto& transform = world_scene->get<Engine::Components::Transform>(static_cast<entt::entity>(entity));
	physx::PxMaterial* gMaterial = px_physics_->createMaterial(friction, friction, restitution);
	physx::PxShape* shape = px_physics_->createShape(physx::PxCapsuleGeometry(radius, half_extent), *gMaterial);
	physx::PxTransform relative_pose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	shape->setLocalPose(relative_pose);
	gMaterial->release();

	phys_body.body = AddBody(shape, type, transform);
}

void PhysicsSystem_PhysX::AddPlaneBody(Engine::GameObject entity) {
	auto world_scene = reinterpret_cast<entt::registry*>(SystemsAPI::World()->GetObjectsList());
	auto& phys_body = world_scene->get_or_emplace<Engine::Components::PhysicsBody>(static_cast<entt::entity>(entity));
	auto& transform = world_scene->get<Engine::Components::Transform>(static_cast<entt::entity>(entity));
	physx::PxMaterial* gMaterial = px_physics_->createMaterial(1, 1, 1);
	physx::PxShape* shape = px_physics_->createShape(physx::PxPlaneGeometry(), *gMaterial);
	physx::PxTransform relative_pose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	shape->setLocalPose(relative_pose);
	gMaterial->release();

	phys_body.body = AddBody(shape, PhysicsType::STATIC, transform);
}

physx::PxRigidActor* PhysicsSystem_PhysX::AddBody(physx::PxShape* shape, PhysicsType type, Engine::Components::Transform& transform) {
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
	px_scene_->addActor(*px_body);

	shape->release();

	return px_body;
}

bool Engine::Internal::PhysicsSystem_PhysX::Update(float dt) {
	if (simulating_) {
		accumulator_ += dt;
		if (accumulator_ < step_size_) {
			return false;
		}

		do {
			accumulator_ -= step_size_;
			px_scene_->simulate(step_size_);
			px_scene_->fetchResults(true);
		} while (accumulator_ >= step_size_);

		return true;
	}
}


