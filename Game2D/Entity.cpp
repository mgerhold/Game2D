#include "Entity.h"

#include "EntityContainer.h"

Entity::Entity(EntityContainer* entityContainer)
    : mEntityContainer(entityContainer)
{}

void Entity::setParent(Entity& entity) {
    mParent = &entity;
}

void Entity::awake() {
    for (const auto& component : mComponents)
        component->awake();
}

void Entity::draw(const Window& window, RenderStates states) const {
    states.transform *= getWorldTransform();
    for (const auto& component : mComponents)
        window.draw(*component, states);
}

void Entity::update(Time dt) {
    for (const auto& component : mComponents)
        component->update(dt);
}

void Entity::setID(int id) {
    mID = id;
}

int Entity::getID() const {
    return mID;
}

void Entity::addComponent(Component::Ptr component) {
    component->setEntity(this);
    mComponents.push_back(std::move(component));
}

Transform Entity::getWorldTransform() const {
    Transform transform;
    if (mParent)
        transform = mParent->getWorldTransform();
    transform *= getTransform();
    return transform;
}

EntityContainer* Entity::getEntityContainer() const {
    return mEntityContainer;
}
