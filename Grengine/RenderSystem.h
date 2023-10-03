#pragma once
#include "Subsystem.h"
#include <cstdint>
// Abstract interface into a potential render system
// The RenderSystem is "A window and a surface that we can draw stuff to"
// It might make sense to separate the Window and the Renderer, like SFML does
namespace Grengine {

	struct GR_WindowEvent;

	class RenderSystem : public Subsystem
	{
	public:
		/// <summary>
		/// Base on SDL's Window Event ID, this enum is used for the abstraction
		/// of window events we might need to handle
		/// </summary>
		enum class GR_WINDOWEVENT {
            WINDOWEVENT_NONE,           /**< Never used */
            WINDOWEVENT_SHOWN,          /**< Window has been shown */
            WINDOWEVENT_HIDDEN,         /**< Window has been hidden */
            WINDOWEVENT_EXPOSED,        /**< Window has been exposed and should be redrawn */
            
			WINDOWEVENT_MOVED,          /**< Window has been moved to data1, data2*/
			
			WINDOWEVENT_RESIZED,        /**< Window has been resized to data1xdata2 */
			WINDOWEVENT_SIZE_CHANGED,   /**< The window size has changed, either as a result of an API call or through the system or user changing the window size. */
			
			WINDOWEVENT_MINIMIZED,      /**< Window has been minimized */
			WINDOWEVENT_MAXIMIZED,      /**< Window has been maximized */
			WINDOWEVENT_RESTORED,       /**< Window has been restored to normal size and position */
			
			WINDOWEVENT_ENTER,          /**< Window has gained mouse focus */
			WINDOWEVENT_LEAVE,          /**< Window has lost mouse focus */
			WINDOWEVENT_FOCUS_GAINED,   /**< Window has gained keyboard focus */
			WINDOWEVENT_FOCUS_LOST,     /**< Window has lost keyboard focus */
			WINDOWEVENT_CLOSE,          /**< The window manager requests that the window be closed */
			WINDOWEVENT_TAKE_FOCUS,     /**< Window is being offered a focus (should SetWindowInputFocus() on itself or a subwindow, or ignore) */
			WINDOWEVENT_HIT_TEST,       /**< Window had a hit test that wasn't SDL_HITTEST_NORMAL. */
			WINDOWEVENT_ICCPROF_CHANGED,/**< The ICC profile of the window's display has changed. */
			WINDOWEVENT_DISPLAY_CHANGED /**< Window has been moved to display data1. */
		};

	public:
		//RenderSystem interface
		virtual int		OpenWindow(int width, int height)		= 0;
		virtual int		CreateRenderer()						= 0;
		virtual void	Clear()									= 0;
		virtual void	Display()								= 0;
		virtual void	DrawLine(int x1, int y1, int x2, int y2) = 0;
		virtual void	HandleWindowEvent(GR_WindowEvent& e)	= 0;
	};

	//Global pointer to the RenderSystem
	extern RenderSystem* grRenderSystem;

	/**
	 * Based on the SDL_WindowEvent
	 * This should really be in its own file and could be a bit more abstracted.
	 * You get the idea though
	 */
	typedef struct GR_WindowEvent
	{
		uint32_t	type;						/**< Overall event type. Nothing right now */
		uint32_t	timestamp;					/**< In milliseconds, not populated at all! */
		uint32_t	windowID;					/**< The associated window */
		RenderSystem::GR_WINDOWEVENT event;     /**< ::GR_WINDOWEVENT */
		uint8_t		padding1;					//TODO: Check the padding on this
		uint8_t		padding2;
		uint8_t		padding3;
		int32_t		data1;						/**< event dependent data */
		int32_t		data2;						/**< event dependent data */
	} GR_WindowEvent;
}
