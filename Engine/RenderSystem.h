#pragma once
#include "Subsystem.h"
#include <cstdint>
// Abstract interface into a potential render system
// The RenderSystem is "A window and a surface that we can draw stuff to"
// It might make sense to separate the Window and the Renderer, like SFML does
namespace Engine::Internal {

	struct WindowEvent;

	class RenderSystem : public Subsystem {
	public:
		//RenderSystem interface
		RenderSystem() { type_ = SubsytemType::RENDER; }
		virtual int		CreateRenderer()						= 0;
		virtual void	Clear()									= 0;
		virtual void	Render()									= 0;
		virtual void	Display()								= 0;
		virtual void	DrawLine(int x1, int y1, int x2, int y2) = 0;
	};
}
