#include "pch.h"
#include "UserInterfaceSystem.h"
#include "SystemsLocator.h"

int UserInterfaceSystem::CreateFontTexture(std::string font, std::string text, int pt_size, int& wrap, float r, float g, float b) {
    return ToffeeAPI::Resources()->CreateFontTexture(font, text, pt_size, wrap, r, g, b);
}
