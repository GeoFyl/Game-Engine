#pragma once
#include "Subsystem.h"
#include "Components.h"

namespace Engine::Internal {
	class WorldSystem : public Subsystem {
	public:
		WorldSystem() { type_ = SubsytemType::WORLD; }
		virtual void* GetObjectsList() = 0;
		virtual Engine::GameObject CreateGameObject() = 0;
		virtual Engine::GameObject CreateGameObject(float x, float y, float z) = 0;
		virtual Engine::GameObject CreateDirectionalLight() = 0;
		virtual Engine::GameObject CreateDirectionalLight(float x, float y, float z) = 0;
		virtual Engine::GameObject CreatePositionalLight() = 0;
		virtual Engine::GameObject CreatePositionalLight(float x, float y, float z) = 0;
		virtual Engine::GameObject CreateSpotLight() = 0;
		virtual Engine::GameObject CreateSpotLight(float x, float y, float z, float d_x, float d_y, float d_z) = 0;
		
		virtual void Move(GameObject object, float x, float y, float z) = 0;
		virtual void Rotate(GameObject object, float rot_x, float rot_y, float rot_z) = 0;
		virtual void SetPosition(GameObject object, float x, float y, float z) = 0;
		virtual void SetScale(GameObject entity, float scalex, float scaley, float scalez) = 0;
		virtual void SetScale(GameObject object, float scale_radius, float scale_height) = 0;
		virtual void SetScale(GameObject object, float scale_radius) = 0;
		virtual void SetRotation(GameObject object, float rot_x, float rot_y, float rot_z) = 0;
		virtual void SetDirection(GameObject entity, float x, float y, float z) = 0;
		virtual void SetMesh(GameObject object, std::string name) = 0;
		virtual void SetTexture(GameObject object, std::string name) = 0;
		virtual void SetAmbientMat(GameObject object, float r, float g, float b, float a) = 0;
		virtual void SetDiffuseMat(GameObject object, float r, float g, float b, float a) = 0;
		virtual void SetAmbientSource(GameObject object, float r, float g, float b, float a) = 0;
		virtual void SetDiffuseSource(GameObject object, float r, float g, float b, float a) = 0;
		virtual Engine::Components::Transform GetPosition(GameObject object) = 0;

	protected:
		friend class Core;
		virtual void Update() = 0;
	};
}
