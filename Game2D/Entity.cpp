#include "Entity.h"

Entity::Entity()
    : mParent(nullptr)
    , mID(-1)
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

template<typename T>
T* Entity::getComponent() const {
    for (const auto& component : mComponents) {
        T* result = dynamic_cast<T*>(component.get());
        if (result)
            return result;
    }
    return nullptr;
}
