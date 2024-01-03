#include "pch.h"
#include "UserInterfaceSystem_SDL_GL.h"
#include "SystemsLocator.h"
#include "ResourceSystem_MiniZip.h"

using namespace Engine::Internal;

UserInterfaceSystem_SDL_GL* ui_entt = new UserInterfaceSystem_SDL_GL;

UserInterfaceSystem_SDL_GL::UserInterfaceSystem_SDL_GL() {
	ProvideSystem(this);
	ui_entt = nullptr;
}

int UserInterfaceSystem_SDL_GL::Initialise() {
	TTF_Init();
	return true;
}

int UserInterfaceSystem_SDL_GL::Shutdown() {
	return 0;
}

UIElement_ID UserInterfaceSystem_SDL_GL::AddElement(int pos_x, int pos_y, int width, int height, std::string text, bool visible) {
	UIElement elem;
	elem.pos_x = pos_x;
	elem.pos_y = pos_y;
	elem.width = width;
	elem.height = height;
	elem.visible = visible;
	if (!text.empty()) {
		elem.text = text;
		elem.texture = SystemsAPI::Resources()->GetResource(text);
	}

	ui_elements_.push_back(std::move(elem));
	return ui_elements_.size() - 1;
}

UIElement_ID Engine::Internal::UserInterfaceSystem_SDL_GL::AddElement(UIElement& elem)
{
	ui_elements_.push_back(std::move(elem));
	return ui_elements_.size() - 1;
}

UIElement_ID Engine::Internal::UserInterfaceSystem_SDL_GL::AddText(int pos_x, int pos_y, int width, int pt_size, std::string font, std::string text, bool visible, float r, float g, float b)
{
	int height = reinterpret_cast<ResourceSystem_MiniZip*>(SystemsAPI::Resources())->CreateFontTexture(font, text, pt_size, width, r, g, b);
	if (height == -1) return -1;
	return AddElement(pos_x, pos_y, width, height, text);
}

void UserInterfaceSystem_SDL_GL::SetText(UIElement_ID elem, int width, int pt_size, std::string font, std::string text, float r, float g, float b) {
	if (elem < 0) return;
	UIElement& element = ui_elements_[elem];
	SystemsAPI::Resources()->UnLoad(element.text);
	element.text = text;
	element.width = width;
	element.height = reinterpret_cast<ResourceSystem_MiniZip*>(SystemsAPI::Resources())->CreateFontTexture(font, text, pt_size, element.width, r, g, b);
	if (element.height == -1) return;
	element.texture = SystemsAPI::Resources()->GetResource(text);
}

bool UserInterfaceSystem_SDL_GL::IsMouseOver(UIElement_ID id) {
	if (id < 0) return false;
	int mouse_x = SystemsAPI::Input()->GetMouseX();
	int mouse_y = SystemsAPI::Input()->GetMouseY();
	UIElement& elem = ui_elements_[id];
	
	return (mouse_x > (elem.pos_x - elem.width / 2) && mouse_x < (elem.pos_x + elem.width / 2) && mouse_y >(elem.pos_y - elem.height / 2) && mouse_y < (elem.pos_y + elem.height / 2));
}



