#include "Entity.h"

#include "EntityContainer.h"
#include "ComponentWithSize.h"

Entity::Entity(EntityContainer* entityContainer, AppContext context)
    : mEntityContainer(entityContainer)
    , mContext(context)
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

bool Entity::handleEvent(Event e) {
    for (const auto& component : mComponents) {
        if (component->handleEvent(e))
            return true;
    }
    return false;
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

FloatRect Entity::getWorldBounds() const {
    auto transform = getWorldTransform();
    auto local = getLocalBounds();
    glm::vec4 p1 = glm::vec4(local.left, local.bottom, 0.f, 1.f);
    glm::vec4 p2 = glm::vec4(local.right, local.top, 0.f, 1.f);
    p1 = transform.getMatrix() * p1;
    p2 = transform.getMatrix() * p2;
    
    FloatRect result;
    result.left = p1.x;
    result.bottom = p1.y;
    result.right = p2.x;
    result.top = p2.y;
    return result;
}

EntityContainer* Entity::getEntityContainer() const {
    return mEntityContainer;
}

glm::ivec2 Entity::getSize() const {
    auto componentWithSize = getComponent<ComponentWithSize>();
    if (componentWithSize)
        return componentWithSize->determineSize();
    else 
        return glm::ivec2(0);
}

AppContext Entity::getContext() const {
    return mContext;
}
