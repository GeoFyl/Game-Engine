#include "pch.h"
#include "UserInterfaceSystem_.h"
#include "SystemsLocator.h"
#include "ResourceSystem_MiniZip.h"

using namespace Toffee::Internal;

UserInterfaceSystem_* ui_entt = new UserInterfaceSystem_;

UserInterfaceSystem_::UserInterfaceSystem_() {
	ProvideSystem(this);
	ui_entt = nullptr;
}

// Init sdl ttf library
int UserInterfaceSystem_::Initialise() {
	TTF_Init();
	return true;
}

// Free memory
int UserInterfaceSystem_::Shutdown() {
	TTF_Quit();
	return 0;
}

// Add a UI element
Toffee::UIElement_ID UserInterfaceSystem_::AddElement(int pos_x, int pos_y, int width, int height, std::string texture, bool visible) {
	// Create element structure with given variables 
	UIElement elem;
	elem.pos_x = pos_x;
	elem.pos_y = pos_y;
	elem.width = width;
	elem.height = height;
	elem.visible = visible;

	if (!texture.empty()) { // If a texture was specified, retreive and store handle to it
		elem.text = texture;
		elem.texture = ToffeeAPI::Resources()->GetResource(texture);
	}

	// Move to elements list
	ui_elements_.push_back(std::move(elem));
	return ui_elements_.size() - 1;
}

// Add a text element
Toffee::UIElement_ID UserInterfaceSystem_::AddText(int pos_x, int pos_y, int width, int pt_size, std::string font, std::string text, bool visible, float r, float g, float b) {
	// Make the resource system create a text texture with given variables 
	// The actual string being represented doubles as the name of the texture
	// Width is the max width, text will wrap to fit
	int height = CreateFontTexture(font, text, pt_size, width, r, g, b);
	if (height == -1) return -1;
	// If successful add a new element using that texture.
	return AddElement(pos_x, pos_y, width, height, text);
}

// Update a text element with new text
void UserInterfaceSystem_::SetText(UIElement_ID elem, int width, int pt_size, std::string font, std::string text, float r, float g, float b) {
	if (elem < 0) return;
	UIElement& element = ui_elements_[elem]; // Get reference to the element
	ToffeeAPI::Resources()->UnLoad(element.text); // Destroy the old texture
	// Update variables
	element.text = text;
	element.width = width;
	element.height = CreateFontTexture(font, text, pt_size, element.width, r, g, b); // Make the resource system create a new text texture with given variables
	if (element.height == -1) return;
	// Retreive and store handle to the new texture
	element.texture = ToffeeAPI::Resources()->GetResource(text);
}

// Returns whether the mouse is over an element.
bool UserInterfaceSystem_::IsMouseOver(UIElement_ID id) {
	if (id < 0) return false; // Element doesnt exist

	// Get mouse coordinates and reference to element
	int mouse_x = ToffeeAPI::Input()->GetMouseX();
	int mouse_y = ToffeeAPI::Input()->GetMouseY();
	UIElement& elem = ui_elements_[id];
	
	// Return true if mouse coordinates are within the bounding box of the UI element
	return (mouse_x > (elem.pos_x - elem.width / 2) && mouse_x < (elem.pos_x + elem.width / 2) && mouse_y >(elem.pos_y - elem.height / 2) && mouse_y < (elem.pos_y + elem.height / 2));
}



