#pragma once
#include "UserInterfaceSystem.h"

namespace Toffee::Internal {
    class UserInterfaceSystem_ : public UserInterfaceSystem {
    public:
        UserInterfaceSystem_();

    private:
        // Inherited from Subsystem

        int Initialise() final;
        int Shutdown() final;

        // See UserInterfaceSystem.h for function details

        UIElement_ID AddElement(int pos_x, int pos_y, int width, int height, std::string texture = "", bool visible = true) final;
        UIElement_ID AddText(int pos_x, int pos_y, int width, int pt_size, std::string font, std::string text, bool visible = true, float r = 1.f, float g = 1.f, float b = 1.f) final;
        void SetText(UIElement_ID elem, int width, int pt_size, std::string font, std::string text, float r = 1.f, float g = 1.f, float b = 1.f) final;
        std::vector<UIElement>* GetElements() final { return &ui_elements_; }
        void SetVisible(UIElement_ID elem, bool val) final { if (elem >= 0) ui_elements_[elem].visible = val; }
        bool IsMouseOver(UIElement_ID);

    protected:
        std::vector<UIElement> ui_elements_;

    };
}
