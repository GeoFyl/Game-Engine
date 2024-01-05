#pragma once
#include "Subsystem.h"
#include "Components.h"

namespace Toffee::Internal {
	// Interface base class to world system.
	class WorldSystem : public Subsystem {
	public:
		WorldSystem() { type_ = SubsytemType::WORLD; }

		// Get reference to list of objects in the current scene.
		virtual void* GetObjectsList() = 0;

		// Create an empty game object at (0,0,0).
		virtual Toffee::GameObject CreateGameObject() = 0;
		// Create an empty game object at (x,y,z).
		virtual Toffee::GameObject CreateGameObject(float x, float y, float z) = 0;
		// Create directional light pointing in direction (1,0,0).
		virtual Toffee::GameObject CreateDirectionalLight() = 0;
		// Create directional light pointing in direction (x,y,z).
		virtual Toffee::GameObject CreateDirectionalLight(float x, float y, float z) = 0;
		// Create a point light at (0,0,0).
		virtual Toffee::GameObject CreatePointLight() = 0;
		// Create a point light at (x,y,z).
		virtual Toffee::GameObject CreatePointLight(float x, float y, float z) = 0;
		// Create a spot light at (0,0,0) pointing straight down.
		virtual Toffee::GameObject CreateSpotLight() = 0;
		// Create a spot light at (0,0,0) pointing in direction (x,y,z).
		virtual Toffee::GameObject CreateSpotLight(float x, float y, float z, float d_x, float d_y, float d_z) = 0;

		// Translate an object's position by (x,y,z) amount.
		virtual void Move(GameObject object, float x, float y, float z) = 0;
		// Rotate an object around its local axis.
		virtual void Rotate(GameObject object, float rot_x, float rot_y, float rot_z) = 0;
		// Set an object's position.
		virtual void SetPosition(GameObject object, float x, float y, float z) = 0;
		// Set an object's scale.
		virtual void SetScale(GameObject entity, float scalex, float scaley, float scalez) = 0;
		// Set scale of object with a capsule physics body
		virtual void SetScale(GameObject object, float scale_radius, float scale_height) = 0;
		// Set scale of object with a sphere physics body
		virtual void SetScale(GameObject object, float scale_radius) = 0;
		// Set an object's rotation.
		virtual void SetRotation(GameObject object, float rot_x, float rot_y, float rot_z) = 0;
		// Set a light source's direction.
		virtual void SetDirection(GameObject entity, float x, float y, float z) = 0;
		// Set an object's mesh.
		virtual void SetMesh(GameObject object, std::string name) = 0;
		// Set an object's texture.
		virtual void SetTexture(GameObject object, std::string name) = 0;
		// Set the ambient component of an object's material.
		virtual void SetAmbientMat(GameObject object, float r, float g, float b, float a) = 0;
		// Set the diffuse component of an object's material.
		virtual void SetDiffuseMat(GameObject object, float r, float g, float b, float a) = 0;
		// Set the ambient component of a light source.
		virtual void SetAmbientSource(GameObject object, float r, float g, float b, float a) = 0;
		// Set the diffuse component of a light source.
		virtual void SetDiffuseSource(GameObject object, float r, float g, float b, float a) = 0;

		// Create a new scene
		virtual void NewScene(std::string name) = 0;
		// Get the current scene
		virtual std::string GetScene() = 0;
		// Check if a scene exists
		virtual bool SceneExists(std::string name) = 0;
		// Switch to a specified scene (create it if it doesn't exist yet).
		virtual void SwitchScene(std::string name) = 0;
		// Destroy a scene and all objects in it.
		virtual void DestroyScene(std::string name) = 0;

	protected:
		friend class Core;
		virtual void Update() = 0;

		// Get the physics system to switch to the specified scene.
		void SwitchPhysicsScene(std::string scene);
	};
}
