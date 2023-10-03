#pragma once
#include "RenderSystem.h"

//Forward declarations
class SDL_Window;
class SDL_Renderer;

namespace Grengine {
    class RenderSystem_SDL :
        public RenderSystem
    {
	public:
		RenderSystem_SDL();
		~RenderSystem_SDL();

		//Inherited from Subsystem interface
		int			Initialise() final;
		int			Shutdown() final;

	public:
		//Inherited from RenderSystem interface
		int			OpenWindow(int width, int height) final;
		int			CreateRenderer() final;
		void		Clear() final;
		void		Display() final;
		void		DrawLine(int x1, int y1, int x2, int y2) final;
		void		HandleWindowEvent(GR_WindowEvent& e) final;

		inline int	GetScreenWidth()		const { return m_ScreenWidth; };
		inline int	GetScreenHeight()	const { return m_ScreenHeight; };

	private:
		SDL_Window*		m_Window;
		SDL_Renderer*	m_Renderer;
		
		int				m_ScreenWidth;
		int				m_ScreenHeight;
    };
}

