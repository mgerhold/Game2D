#include "Button.h"

GUI::Button::Button()
	: GUIComponent()
	, mTextureNormal(nullptr)
	, mTextureSelected(nullptr)
	, mTextureActive(nullptr)
	, mIsHeldDown(false)
{}

bool GUI::Button::isSelectable() const {
	return true;
}

bool GUI::Button::handleEvent(const Event& event, const Window& window) {
	bool inside = false;
	if (event.type == Event::Type::MouseMove || event.type == Event::Type::MouseButtonPress || event.type == Event::Type::MouseButtonRelease)
		inside = isMouseInside(event, window);

	if (event.type == Event::Type::MouseMove) {
		if (inside && !isSelected()) {
			// mouse just moved inside
			select();
			if (mIsHeldDown) {
				activate();
				mIsHeldDown = false;
			}
			updateSprite();
			return true;
		} else if (!inside && isSelected()) {
			// mouse just moved outside
			deselect();
			if (isActive()) {
				deactivate();
				mIsHeldDown = true;
			}
			updateSprite();
		}
	} else if (event.type == Event::Type::MouseButtonPress && event.mouseButton == MouseButton::Left) {
		if (inside && !isActive()) {
			// just clicked the button
			activate();
			updateSprite();
			return true;
		}
	} else if (event.type == Event::Type::MouseButtonRelease && event.mouseButton == MouseButton::Left) {
		if (inside && isActive()) {
			// triggered button
			deactivate();
			updateSprite();
			if (mCallbackFunc)
				mCallbackFunc();
		} else if (!inside) {
			mIsHeldDown = false;
		}
	}
	return false;
}

void GUI::Button::draw(const Window& window, RenderStates states) const {
	states.transform *= getTransform();
	window.draw(mSprite, states);
	window.draw(mText, states);
}

void GUI::Button::setString(const std::string& string) {
	mText.setString(string);
	centerText();
}

void GUI::Button::setFont(Font& font, unsigned int fontHeight) {
	mText.setFont(font, fontHeight);
	centerText();
}

void GUI::Button::setFontColor(Color color) {
	mText.setColor(color);
}

glm::ivec2 GUI::Button::getSize() const {
	if (mTextureNormal)
		return mTextureNormal->getSize();
	else
		return glm::ivec2(0);
}

void GUI::Button::setNormalTexture(const Texture& texture) {
	mTextureNormal = &texture;
	updateSprite();
	centerText();
}
void GUI::Button::setSelectedTexture(const Texture& texture) {
	mTextureSelected = &texture;
	updateSprite();
}

void GUI::Button::setActiveTexture(const Texture& texture) {
	mTextureActive = &texture;
	updateSprite();
}

void GUI::Button::setCallbackFunc(CallbackFunc func) {
	mCallbackFunc = func;
}

void GUI::Button::updateSprite() {
	State state = State::Normal;
	if (isSelected())
		state = State::Selected;
	if (isActive())
		state = State::Active;
	switch (state) {
		case State::Normal:
			if (mTextureNormal)
				mSprite.setTexture(*mTextureNormal);
			break;
		case State::Selected:
			if (mTextureSelected)
				mSprite.setTexture(*mTextureSelected);
			break;
		case State::Active:
			if (mTextureActive)
				mSprite.setTexture(*mTextureActive);
			break;
	}
}

void GUI::Button::centerText() {
	mText.centerOrigin();
	if (mTextureNormal)
		mText.setPosition(mTextureNormal->getSize() / 2);
}

bool GUI::Button::isMouseInside(const Event& event, const Window& window) const {
	auto pos = window.windowToWorldCoords(glm::vec2(event.mousePosition.x, event.mousePosition.y));
	return getLocalBounds().isInside(pos.x, pos.y);
}
