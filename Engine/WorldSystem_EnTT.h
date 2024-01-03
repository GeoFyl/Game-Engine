#pragma once
#include "WorldSystem.h"
#include <entt/entity/registry.hpp>

namespace Engine::Internal {
    class WorldSystem_EnTT : public WorldSystem {
    public:
        WorldSystem_EnTT();
        int Initialise() final;
        int Shutdown() final;
        void* GetObjectsList() { return &registry_; }
        Engine::GameObject CreateGameObject();
        Engine::GameObject CreateGameObject(float x, float y, float z);
        Engine::GameObject CreateDirectionalLight();
        Engine::GameObject CreateDirectionalLight(float x, float y, float z);
        Engine::GameObject CreatePositionalLight();
        Engine::GameObject CreatePositionalLight(float x, float y, float z);
        Engine::GameObject CreateSpotLight();
        Engine::GameObject CreateSpotLight(float x, float y, float z, float d_x, float d_y, float d_z);
        
        void Move(GameObject entity, float x, float y, float z);
        void Rotate(GameObject object, float rotx, float roty, float rotz);
        void SetPosition(GameObject entity, float x, float y, float z);
        void SetDirection(GameObject entity, float x, float y, float z);
        void SetScale(GameObject entity, float scalex, float scaley, float scalez);
        void SetScale(GameObject object, float scale_radius, float scale_height);
        void SetScale(GameObject object, float scale_radius);
        void SetRotation(GameObject entity, float rotx, float roty, float rotz);
        void SetMesh(GameObject object, std::string name);
        void SetTexture(GameObject object, std::string name);
        void SetAmbientMat(GameObject object, float r, float g, float b, float a);
        void SetDiffuseMat(GameObject object, float r, float g, float b, float a);
        void SetAmbientSource(GameObject object, float r, float g, float b, float a);
        void SetDiffuseSource(GameObject object, float r, float g, float b, float a);

        Engine::Components::Transform GetPosition(GameObject entity);
    private:
        void Update();
        void CopyMatrix(float* mat_1, const float* mat_2);

        entt::registry registry_;

        int num_lights = 0;
    };
}
