#include "GUIComponent.h"

GUI::GUIComponent::GUIComponent()
	: mIsSelected(false)
	, mIsActive(false)
{}

bool GUI::GUIComponent::isSelected() const {
	return mIsSelected;
}

void GUI::GUIComponent::select() {
	if (isSelectable())
		mIsSelected = true;
}

void GUI::GUIComponent::deselect() {
	mIsSelected = false;
}

void GUI::GUIComponent::activate() {
	mIsActive = true;
}

void GUI::GUIComponent::deactivate() {
	mIsActive = false;
}

bool GUI::GUIComponent::isActive() const {
	return mIsActive;
}

bool GUI::GUIComponent::handleEvent(const Event& event, const Window& window) {
	return false;
}
