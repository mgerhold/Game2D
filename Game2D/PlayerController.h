#pragma once

#include "Component.h"

class RigidBody;
class Window;

class PlayerController : public Component {
public:
	bool handleEvent(Event e) override;

private:
	void onAwake() override;
	void onUpdate(Time dt) override;

private:
	RigidBody* mRigidBody = nullptr;
	const Window* mWindow = nullptr;
	bool mIsTouchingMap = false;
};