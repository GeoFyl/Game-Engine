#pragma once
#include "Subsystem.h"
#include "Components.h"



namespace Engine::Internal {

	class PhysicsSystem : public Subsystem {
	public:
		PhysicsSystem() { type_ = SubsytemType::PHYSICS; }
		virtual void SetGravity(float x, float y, float z) = 0;
		virtual void ToggleSimulating() = 0;
		virtual void SetActive(GameObject object, bool value) = 0;
		virtual void ApplyImpulse(GameObject object, float x, float y, float z) = 0;
		virtual void SetVelocity(GameObject object, float x, float y, float z) = 0;
		virtual void SetAngularVelocity(GameObject object, float x, float y, float z) = 0;
		virtual void ApplyVelocity(GameObject object, float x, float y, float z) = 0;
		virtual void ApplyAngularVelocity(GameObject object, float x, float y, float z) = 0;
		virtual void SetAngularDamping(GameObject object, float value) = 0;

		virtual void AddBoxBody(Engine::GameObject object, float half_x = 0.5f, float half_y = 0.5f, float half_z = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f) = 0;
		virtual void AddSphereBody(Engine::GameObject object, float radius = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f) = 0;
		virtual void AddCapsuleBody(Engine::GameObject object, float half_extent = 0.5f, float radius = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f) = 0;
		virtual void AddPlaneBody(Engine::GameObject object) = 0;

	protected:
		friend class Core;
		virtual bool Update(float dt) = 0;

	};

}
