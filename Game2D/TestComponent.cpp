#include "TestComponent.h"
#include <iostream>

void TestComponent::onAwake() {
	std::cout << "TestComponent::onAwake\n";
}

void TestComponent::onUpdate(Time dt) {
	//std::cout << "TestComponent::onUpdate\n";
}

void TestComponent::onDraw(const Window& window, RenderStates states) const {
	//std::cout << "TestComponent::onDraw\n";
}
