#pragma once
#include "RenderSystem.h"

//Forward declarations
class SDL_Window;
class SDL_Renderer;
typedef void* SDL_GLContext;

namespace Engine::Internal {
    class RenderSystem_SDL_GL : public RenderSystem {
	public:
		RenderSystem_SDL_GL();
		~RenderSystem_SDL_GL();

		//Inherited from Subsystem interface
		int			Initialise() final;
		int			Shutdown() final;

	public:
		//Inherited from RenderSystem interface
		
		int			CreateRenderer() final;
		void		Clear() final;
		void		Display() final;
		void		DrawLine(int x1, int y1, int x2, int y2) final;

	private:
		SDL_Window* window_;
		SDL_Renderer* renderer_2D_;
		SDL_GLContext openGL_context_;
    };
}

