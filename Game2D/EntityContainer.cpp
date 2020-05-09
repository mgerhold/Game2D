#include "EntityContainer.h"

EntityContainer::EntityContainer(AppContext context)
	: mContext(context)
{}

void EntityContainer::add(Entity::Ptr entity) {
	mEntities.push_back(std::move(entity));
}

void EntityContainer::awake() {
	for (const auto& entity : mEntities)
		entity->awake();
}

void EntityContainer::draw(const Window& window, RenderStates states) const {
	for (const auto& entity : mEntities)
		window.draw(*entity, states);
}

void EntityContainer::update(Time dt) {
	for (const auto& entity : mEntities)
		entity->update(dt);
}

bool EntityContainer::handleEvent(Event e) {
	for (const auto& entity : mEntities) {
		if (entity->handleEvent(e))
			return true;
	}
	return false;
}

AppContext EntityContainer::getContext() const {
	return mContext;
}
