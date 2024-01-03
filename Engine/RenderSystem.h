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

		virtual void ToggleLighting() = 0;
		virtual void ToggleWireframe() = 0;
		virtual void ToggleVsync() = 0;

	protected:
		friend class Core;
		friend class WindowSystem;
		virtual int		CreateRenderer()						= 0;
		virtual void	Resize(float w, float h) = 0;
		virtual void	Clear()									= 0;
		virtual void	Render()									= 0;
		virtual void	Display()								= 0;
	};
}
