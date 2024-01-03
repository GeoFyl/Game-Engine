#pragma once
#include "pch.h"
#include "RenderSystem.h"
#include "ResourceSystem_MiniZip.h"

//Forward declarations
class SDL_Window;
class SDL_Renderer;
class SDL_Texture;
typedef void* SDL_GLContext;

namespace Engine::Internal {
    class RenderSystem_SDL_GL : public RenderSystem {
	public:
		RenderSystem_SDL_GL();
		~RenderSystem_SDL_GL();

		void ToggleLighting() { lighting_ = !lighting_; }
		void ToggleWireframe() { wireframe_ = !wireframe_; }
		void ToggleVsync();

	private:
		//Inherited from Subsystem interface
		int			Initialise() final;
		int			Shutdown() final;

		//Inherited from RenderSystem interface
		
		int			CreateRenderer() final;
		void		Clear() final;
		void		Display() final;
		void Render() final;

		void Resize(float w, float h);

		SDL_Window* window_;
		SDL_GLContext openGL_context_;

		Engine::Components::Mesh* UI_mesh_;

		float width_;
		float height_;
		float ratio_;
		float fov_ = 45.0f;
		float nearPlane_ = 0.1f;
		float farPlane_ = 100.0f;

		bool lighting_ = true;
		bool wireframe_ = false;
		bool vsync_ = true;
    };
}

