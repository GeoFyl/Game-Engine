#pragma once
#include "RenderSystem.h"
#include "Quad.h"

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

		//Inherited from Subsystem interface
		int			Initialise() final;
		int			Shutdown() final;

		//Inherited from RenderSystem interface
		
		int			CreateRenderer() final;
		void		Clear() final;
		void		Display() final;
		void Render() final;
		void		DrawLine(int x1, int y1, int x2, int y2) final;

	private:
		void Resize();
		void Render2DTarget();

		SDL_Window* window_;
		SDL_Renderer* renderer_2D_;
		SDL_Texture* target_2D_;
		std::unique_ptr<Quad> target_2D_quad_;
		SDL_GLContext openGL_context_;

		float ratio_;
		float fov_ = 45.0f;
		float nearPlane_ = 0.1f;
		float farPlane_ = 100.0f;
    };
}

