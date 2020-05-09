#include "Component.h"
#include "Entity.h"

void Component::draw(const Window& window, RenderStates states) const {
	onDraw(window, states);
}

Component::Component()
	: mEntity(nullptr)
{}

void Component::update(Time dt) {
	onUpdate(dt);
}

bool Component::handleEvent(Event e) {
	return false;
}

void Component::awake() {
	onAwake();
}

Entity* Component::getEntity() {
	return mEntity;
}

void Component::onAwake()
{}

void Component::onDraw(const Window& window, RenderStates states) const
{}

void Component::onUpdate(Time dt)
{}

void Component::setEntity(Entity * entity) {
	mEntity = entity;
}