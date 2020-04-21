#include "Component.h"

GUI::Component::Component()
	: mIsSelected(false)
	, mIsActive(false)
{}

bool GUI::Component::isSelected() const {
	return mIsSelected;
}

void GUI::Component::select() {
	if (isSelectable())
		mIsSelected = true;
}

void GUI::Component::deselect() {
	mIsSelected = false;
}

void GUI::Component::activate() {
	mIsActive = true;
}

void GUI::Component::deactivate() {
	mIsActive = false;
}

bool GUI::Component::isActive() const {
	return mIsActive;
}

bool GUI::Component::handleEvent(const Event& event, const Window& window) {
	return false;
}
