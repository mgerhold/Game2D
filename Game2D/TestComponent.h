#pragma once

#include "Component.h"

class TestComponent : public Component {
private:
	virtual void onAwake() override;
	virtual void onUpdate(Time dt) override;
	virtual void onDraw(const Window& window, RenderStates states) const override;
};