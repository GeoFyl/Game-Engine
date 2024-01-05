#pragma once
#include "pch.h"
#include "RenderSystem.h"
#include "ResourceSystem_MiniZip.h"

//Forward declarations
class SDL_Window;
typedef void* SDL_GLContext;

namespace Toffee::Internal {
    class RenderSystem_SDL_GL : public RenderSystem {
	public:
		RenderSystem_SDL_GL();

	private:

		//Inherited from Subsystem interface

		int			Initialise() final;
		int			Shutdown() final;

		//Inherited from RenderSystem interface

		int			CreateRenderer() final;
		void		Clear() final;
		void		Display() final;
		void Render() final;
		void ToggleLighting() final { lighting_ = !lighting_; }
		void ToggleWireframe() final { wireframe_ = !wireframe_; }
		void ToggleVsync() final;
		void SetLookAt(float cam_x, float cam_y, float cam_z, float look_at_x, float look_at_y, float look_at_z, float up_x = 0, float up_y = 1, float up_z = 0) final;
		void Resize(float w, float h);

		SDL_Window* window_;
		SDL_GLContext openGL_context_;

		Toffee::Components::Mesh* UI_mesh_; // Quad for drawing UI

		float width_;
		float height_;
		float ratio_;
		float fov_ = 45.0f;
		float nearPlane_ = 0.1f;
		float farPlane_ = 100.0f;

		// State variables
		bool lighting_ = true;
		bool wireframe_ = false;
		bool vsync_ = true;
		float look_at_[9] = { 0,0,6,0,0,0,0,1,0 };
    };
}

