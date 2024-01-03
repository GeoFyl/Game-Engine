#pragma once
#include "Subsystem.h"

namespace Engine::Internal {
	typedef int UIElement_ID;

	struct UIElement {
		int pos_x;
		int pos_y;
		int width;
		int height;
		bool visible;
		std::string text;
		void* texture = nullptr;
	};

	class UserInterfaceSystem : public Subsystem
	{
	public:
		UserInterfaceSystem() { type_ = SubsytemType::UI; }
		virtual UIElement_ID AddText(int pos_x, int pos_y, int width, int pt_size, std::string font, std::string text, bool visible = true, float r = 1.f, float g = 1.f, float b = 1.f) = 0;
		virtual UIElement_ID AddElement(int pos_x, int pos_y, int width, int height, std::string texture = "", bool visible = true) = 0;
		virtual void SetText(UIElement_ID elem, int width, int pt_size, std::string font, std::string text, float r = 1.f, float g = 1.f, float b = 1.f) = 0;
		virtual void SetVisible(UIElement_ID elem, bool val) = 0;
		virtual bool IsMouseOver(UIElement_ID) = 0;
		virtual std::vector<UIElement>* GetElements() = 0;

	private:

	};
}
