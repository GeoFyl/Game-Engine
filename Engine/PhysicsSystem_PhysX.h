#pragma once
#include "PhysicsSystem.h"
#include <PxPhysicsAPI.h>
#include "Components.h"
#include <unordered_map>

namespace Toffee::Internal {
	class PhysicsSystem_PhysX : public PhysicsSystem {
	public:
		PhysicsSystem_PhysX();

	private:
		// Inherited from Subsystem

		int Initialise() final;
		int Shutdown() final;

		// See PhysicsSystem.h for function details

		void SetGravity(float x, float y, float z) final;
		void ToggleSimulating() final { simulating_ = !simulating_; }
		bool Simulating() final { return simulating_; }
		void SetActive(GameObject object, bool value) final;

		void ApplyImpulse(GameObject object, float x, float y, float z) final;
		void ApplyVelocity(GameObject object, float x, float y, float z) final;
		void ApplyAngularVelocity(GameObject object, float x, float y, float z) final;
		void SetVelocity(GameObject object, float x, float y, float z) final;
		void SetAngularVelocity(GameObject object, float x, float y, float z) final;
		void SetAngularDamping(GameObject object, float value) final;

		void AddBoxBody(Toffee::GameObject object, float half_x = 0.5f, float half_y = 0.5f, float half_z = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f) final;
		void AddSphereBody(Toffee::GameObject object, float radius = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f) final;
		void AddCapsuleBody(Toffee::GameObject object, float half_extent = 0.5f, float radius = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f) final;
		void AddPlaneBody(Toffee::GameObject object) final;

		bool Update(float dt) final;
		void SwitchScene(std::string) final;

		// Add a body to the PhysX scene.
		physx::PxRigidActor* AddBody(physx::PxShape* shape, PhysicsType type, Toffee::Components::Transform& transform);

		physx::PxDefaultAllocator px_allocator_;
		physx::PxDefaultErrorCallback px_error_callback_;
		physx::PxFoundation* px_foundation_ = NULL;
		physx::PxPhysics* px_physics_ = NULL;
		physx::PxDefaultCpuDispatcher* px_dispatcher_ = NULL;
		physx::PxSceneDesc* px_scene_desc_ = NULL;
		std::unordered_map<std::string, physx::PxScene*> px_scenes_;
		physx::PxScene* current_px_scene_;
		float accumulator_ = 0.0f;
		float step_size_ = 1.0f / 360.0f;
		bool simulating_ = false;
	};

}