#pragma once
#include "UserInterfaceSystem.h"

namespace Engine::Internal {
    class UserInterfaceSystem_SDL_GL : public UserInterfaceSystem {
    public:
        UserInterfaceSystem_SDL_GL();
        int Initialise() final;
        int Shutdown() final;

        void Update();
        UIElement_ID AddElement(int pos_x, int pos_y, int width, int height, std::string texture = "", bool visible = true);
        UIElement_ID AddElement(UIElement& elem);
        UIElement_ID AddText(int pos_x, int pos_y, int width, int pt_size, std::string font, std::string text, bool visible = true, float r = 1.f, float g = 1.f, float b = 1.f);
        void SetText(UIElement_ID elem, int width, int pt_size, std::string font, std::string text, float r = 1.f, float g = 1.f, float b = 1.f);
        std::vector<UIElement>* GetElements() { return &ui_elements_; }
        void SetVisible(UIElement_ID elem, bool val) { ui_elements_[elem].visible = val; }
        bool IsMouseOver(UIElement_ID);
    protected:
        std::vector<UIElement> ui_elements_;

    };
}
