#pragma once
#include "Subsystem.h"
#include "Components.h"



namespace Toffee::Internal {
	// Interface base class to physics system.
	class PhysicsSystem : public Subsystem {
	public:
		PhysicsSystem() { type_ = SubsytemType::PHYSICS; }
		// Set global gravity.
		virtual void SetGravity(float x, float y, float z) = 0;
		// Start/stop simulation.
		virtual void ToggleSimulating() = 0;
		// Returns whether simulation currently running.
		virtual bool Simulating() = 0;
		// Activate/deactivate simulation of an object.
		virtual void SetActive(GameObject object, bool value) = 0;
		// Apply an impulse to an object.
		virtual void ApplyImpulse(GameObject object, float x, float y, float z) = 0;
		// Set an object's velocity.
		virtual void SetVelocity(GameObject object, float x, float y, float z) = 0;
		// Set the angular velocity of an object.
		virtual void SetAngularVelocity(GameObject object, float x, float y, float z) = 0;
		// Add a vector to an object's velocity.
		virtual void ApplyVelocity(GameObject object, float x, float y, float z) = 0;
		// Add a vector to an object's angular velocity.
		virtual void ApplyAngularVelocity(GameObject object, float x, float y, float z) = 0;
		// Set angular damping of an object.
		virtual void SetAngularDamping(GameObject object, float value) = 0;
		///<summary> Add a box shaped physics body to a game object. </summary>
		///<param name="half_x">Half width, before scaling</param>
		///<param name="half_y">Half height, before scaling</param>
		///<param name="half_z">Half depth, before scaling</param>
		///<param name="type">Whether the object is static/dynamic</param>
		///<param name="friction">Coefficient of friction</param>
		///<param name="restitution">Coefficient of restitution</param>
		virtual void AddBoxBody(Toffee::GameObject object, float half_x = 0.5f, float half_y = 0.5f, float half_z = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f) = 0;
		///<summary> Add a sphere shaped physics body to a game object. </summary>
		///<param name="radius">The radius, before scaling</param>
		///<param name="type">Whether the object is static/dynamic</param>
		///<param name="friction">Coefficient of friction</param>
		///<param name="restitution">Coefficient of restitution</param>
		virtual void AddSphereBody(Toffee::GameObject object, float radius = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f) = 0;
		///<summary> Add a capsule shaped physics body to a game object. </summary>
		///<param name="half_height">Half height, before scaling</param>
		///<param name="radius">The radius, before scaling</param>
		///<param name="type">Whether the object is static/dynamic</param>
		///<param name="friction">Coefficient of friction</param>
		///<param name="restitution">Coefficient of restitution</param>
		virtual void AddCapsuleBody(Toffee::GameObject object, float half_height = 0.5f, float radius = 0.5f, PhysicsType type = PhysicsType::DYNAMIC, float friction = 0.5f, float restitution = 0.2f) = 0;
		// Add an infinite, horizontal static plane shaped physics body to a game object.
		virtual void AddPlaneBody(Toffee::GameObject object) = 0;

	protected:
		friend class Core;
		friend class WorldSystem;
		// Update the simulation
		virtual bool Update(float dt) = 0;
		// Switch active scene.
		virtual void SwitchScene(std::string) = 0;
	};

}
