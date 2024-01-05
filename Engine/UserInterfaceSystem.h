#pragma once
#include "Subsystem.h"

namespace Toffee {
	typedef int UIElement_ID;
}

namespace Toffee::Internal {

	// Contains data for displaying a UI element.
	struct UIElement {
		int pos_x;
		int pos_y;
		int width;
		int height;
		bool visible;
		std::string text;
		void* texture = nullptr;
	};

	// Interface base class to UI system.
	class UserInterfaceSystem : public Subsystem {
	public:
		UserInterfaceSystem() { type_ = SubsytemType::UI; }
		///<summary>Add a text element.</summary>
		///<param name="pos_x, pos_y">Position of text centre. (0,0) is top left of window.</param>
		///<param name="width">The maximum width of the text. Text will wrap to fit.</param>
		///<param name="pt_size">The point size to use.</param>
		///<param name="font">The font to use.</param>
		///<param name="text">What the text should say.</param>
		///<param name="visible">Whether the text is visible.</param>
		///<param name="r, g, b">The colour.</param>
		virtual UIElement_ID AddText(int pos_x, int pos_y, int width, int pt_size, std::string font, std::string text, bool visible = true, float r = 1.f, float g = 1.f, float b = 1.f) = 0;
		///<summary>Add a UI element.</summary>
		///<param name="pos_x, pos_y">Position of element centre. (0,0) is top left of window.</param>
		///<param name="width, height">Dimensions</param>
		///<param name="texture">Image to use. Can leave empty.</param>
		///<param name="visible">Whether the element is visible.</param>
		virtual UIElement_ID AddElement(int pos_x, int pos_y, int width, int height, std::string texture = "", bool visible = true) = 0;
		///<summary>Update a text element.</summary>
		///<param name="elem">The text element to change.</param>
		///<param name="width">The maximum width of the text. Text will wrap to fit.</param>
		///<param name="pt_size">The point size to use.</param>
		///<param name="font">The font to use.</param>
		///<param name="text">What the text should say.</param>
		///<param name="r, g, b">The colour.</param>
		virtual void SetText(UIElement_ID elem, int width, int pt_size, std::string font, std::string text, float r = 1.f, float g = 1.f, float b = 1.f) = 0;
		// Set a UI element visible/invisible
		virtual void SetVisible(UIElement_ID elem, bool val) = 0;
		// Returns whether the mouse is over an element.
		virtual bool IsMouseOver(UIElement_ID) = 0;

		// Get reference to vector of all UI elements.
		virtual std::vector<UIElement>* GetElements() = 0;

	protected:
		int CreateFontTexture(std::string font, std::string text, int pt_size, int& wrap, float r, float g, float b);
	};
}
