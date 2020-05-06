#pragma once

#include "Drawable.h"
#include "Transformable.h"
#include <vector>
#include "Time.h"
#include "Component.h"

class Entity
	: public Drawable
	, public Transformable
{
public:
	using Ptr = std::unique_ptr<Entity>;

public:
	Entity();

	void	setParent(Entity& entity);
	void awake();
	virtual void update(Time dt) final;
	void setID(int id);
	int getID() const;
	void addComponent(Component::Ptr component);
	template<typename T>
	T*	getComponent() const;
	Transform	getWorldTransform() const;

private:
	virtual void draw(const Window& window, RenderStates states) const final;

private:
	Entity* mParent;
	std::vector<Component::Ptr>	mComponents;
	int mID;
};

template<typename T>
T* Entity::getComponent() const {
	for (const auto& component : mComponents) {
		T* result = dynamic_cast<T*>(component.get());
		if (result)
			return result;
	}
	return nullptr;
}