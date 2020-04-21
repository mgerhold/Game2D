#include "Label.h"

bool GUI::Label::isSelectable() const {
	return true;
}

void GUI::Label::draw(const Window& window, RenderStates states) const {
	states.transform *= getTransform();
	window.draw(mText, states);
}

void GUI::Label::setString(const std::string& string) {
	mText.setString(string);
}

void GUI::Label::setFont(Font& font, unsigned int fontHeight) {
	mText.setFont(font, fontHeight);
}

glm::ivec2 GUI::Label::getSize() const {
	return mText.getSize();
}
