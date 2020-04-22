#pragma once

#include "Component.h"
#include "Text.h"

class TextRenderer : public Component {
public:
	Text&				getText();

private:
	virtual void		onDraw(const Window& window, RenderStates states) const override;


private:
	Text				mText;
};