#pragma once
#include "PhysicsSystem.h"
#include <PxPhysicsAPI.h>
#include "Components.h"

namespace Engine::Internal {
	class PhysicsSystem_PhysX : public PhysicsSystem {
	public:
		PhysicsSystem_PhysX();

		void SetGravity(float x, float y, float z);
		void ToggleSimulating() { simulating_ = !simulating_; }
		void SetActive(GameObject object, bool value);
		void ApplyImpulse(GameObject object, float x, float y, float z);
		void ApplyVelocity(GameObject object, float x, float y, float z);
		void ApplyAngularVelocity(GameObject object, float x, float y, float z);
		void SetVelocity(GameObject object, float x, float y, float z);
		void SetAngularVelocity(GameObject object, float x, float y, float z);
		void SetAngularDamping(GameObject object, float value);

		void AddBoxBody(Engine::GameObject object, float half_x = 0.5f, float half_y = 0.5f, float half_z = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f);
		void AddSphereBody(Engine::GameObject object, float radius = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f);
		void AddCapsuleBody(Engine::GameObject object, float half_extent = 0.5f, float radius = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f);
		void AddPlaneBody(Engine::GameObject object);

	protected:
		friend class WorldSystem_EnTT;

		int	Initialise() final;
		int	Shutdown() final;

		bool Update(float dt);
		physx::PxRigidActor* AddBody(physx::PxShape* shape, PhysicsType type, Engine::Components::Transform& transform);

		physx::PxDefaultAllocator px_allocator_;
		physx::PxDefaultErrorCallback px_error_callback_;
		physx::PxFoundation* px_foundation_ = NULL;
		physx::PxPhysics* px_physics_ = NULL;
		physx::PxDefaultCpuDispatcher* px_dispatcher_ = NULL;
		physx::PxSceneDesc* px_scene_desc_ = NULL;
		physx::PxScene* px_scene_ = NULL;
		float accumulator_ = 0.0f;
		float step_size_ = 1.0f / 360.0f;
		bool simulating_ = false;
	};

}