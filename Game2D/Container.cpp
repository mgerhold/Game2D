#include "Container.h"

bool GUI::Container::isSelectable() const {
	return false;
}

bool GUI::Container::handleEvent(const Event& event, const Window& window) {
	for (const auto& child : mChildren) {
		if (child->handleEvent(event, window))
			return true;
	}
	return false;
}

void GUI::Container::draw(const Window& window, RenderStates states) const {
	states.transform *= getTransform();
	for (const auto& child : mChildren) {		
		window.draw(*child, states);
	}
}

void GUI::Container::pack(GUIComponent::Ptr component) {
	mChildren.push_back(component);
}
