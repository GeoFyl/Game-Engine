#pragma once
#include <stdint.h>

namespace Toffee::Internal {
	/// <summary>
	/// Base on SDL's Window Event ID, this enum is used for the abstraction
	/// of window events we might need to handle
	/// </summary>
	enum class WindowEventID {
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
	struct WindowEvent {
		uint32_t	type;						/**< Overall event type. Nothing right now */
		uint32_t	timestamp;					/**< In milliseconds, not populated at all! */
		uint32_t	windowID;					/**< The associated window */
		WindowEventID event;     /**< ::WindowEventID */
		uint8_t		padding1;					
		uint8_t		padding2;
		uint8_t		padding3;
		int32_t		data1;						/**< event dependent data */
		int32_t		data2;						/**< event dependent data */
	};
}
