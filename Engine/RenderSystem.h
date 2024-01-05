#pragma once
#include "Subsystem.h"
#include <cstdint>

namespace Toffee::Internal {

	// Interface base class to rendering system.
	class RenderSystem : public Subsystem {
	public:
		// RenderSystem interface
		RenderSystem() { type_ = SubsytemType::RENDER; }

		// Turn on/off lighting.
		virtual void ToggleLighting() = 0;
		// Turn on/off wireframe mode.
		virtual void ToggleWireframe() = 0;
		// Turn on/off VSync.
		virtual void ToggleVsync() = 0;
		///<summary> Set the viewing transformation matrix.</summary>
		///<param name="cam_x, cam_y, cam_z">The position of the camera</param>
		///<param name="look_at_x, look_at_y, look_at_z">The position to look towards.</param>
		///<param name="up_x, up_y, up_z">The up direction of the camera.</param>
		virtual void SetLookAt(float cam_x, float cam_y, float cam_z, float look_at_x, float look_at_y, float look_at_z, float up_x = 0, float up_y = 1, float up_z = 0) = 0;
		// Resize the viewport.
		virtual void Resize(float w, float h) = 0;

	protected:
		friend class Core;
		friend class WindowSystem;

		virtual int		CreateRenderer()						= 0;
		virtual void	Clear()									= 0;
		virtual void	Render()									= 0;
		virtual void	Display()								= 0;
	};
}
