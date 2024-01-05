#pragma once
#include "WorldSystem.h"
#include <entt/entity/registry.hpp>
#include <boost/unordered/unordered_flat_map.hpp>

namespace Toffee::Internal {
    class WorldSystem_EnTT : public WorldSystem {
    public:
        WorldSystem_EnTT();

    private:
        // Inherited from Subsystem

        int Initialise() final { return true; };
        int Shutdown() final;

        // See WorldSystem.h for function details

        void* GetObjectsList() final { return &scenes_[current_scene_]; }

        Toffee::GameObject CreateGameObject() final;
        Toffee::GameObject CreateGameObject(float x, float y, float z) final;
        Toffee::GameObject CreateDirectionalLight() final;
        Toffee::GameObject CreateDirectionalLight(float x, float y, float z) final;
        Toffee::GameObject CreatePointLight() final;
        Toffee::GameObject CreatePointLight(float x, float y, float z) final;
        Toffee::GameObject CreateSpotLight() final;
        Toffee::GameObject CreateSpotLight(float x, float y, float z, float d_x, float d_y, float d_z) final;
        
        void Move(GameObject entity, float x, float y, float z) final;
        void Rotate(GameObject object, float rotx, float roty, float rotz) final;
        void SetPosition(GameObject entity, float x, float y, float z) final;
        void SetDirection(GameObject entity, float x, float y, float z) final;
        void SetScale(GameObject entity, float scalex, float scaley, float scalez) final;
        void SetScale(GameObject object, float scale_radius, float scale_height) final;
        void SetScale(GameObject object, float scale_radius) final;
        void SetRotation(GameObject entity, float rotx, float roty, float rotz) final;
        void SetMesh(GameObject object, std::string name) final;
        void SetTexture(GameObject object, std::string name) final;
        void SetAmbientMat(GameObject object, float r, float g, float b, float a) final;
        void SetDiffuseMat(GameObject object, float r, float g, float b, float a) final;
        void SetAmbientSource(GameObject object, float r, float g, float b, float a) final;
        void SetDiffuseSource(GameObject object, float r, float g, float b, float a) final;

        void NewScene(std::string name) final;
        void SwitchScene(std::string name) final;
        void DestroyScene(std::string name) final;
        std::string GetScene() final { return current_scene_; }
        bool SceneExists(std::string name) final { return scenes_.count(name); }

        void Update() final;

        // Copies a matrix
        void CopyMatrix(float* mat_1, const float* mat_2);
        // Keeps track of the current scene.
        std::string current_scene_;
        // Flat map of scenes. We are using an entity component system.
        boost::unordered::unordered_flat_map<std::string, entt::registry> scenes_;
    };
}
