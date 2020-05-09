#pragma once

#include "Drawable.h"
#include "Transformable.h"
#include <vector>
#include "Time.h"
#include "Component.h"
#include "AppContext.h"

class EntityContainer;

class Entity
	: public Drawable
	, public Transformable
{
public:
	using Ptr = std::unique_ptr<Entity>;

public:
								Entity(EntityContainer* entityContainer, AppContext context);

	void						setParent(Entity& entity);
	void						awake();
	virtual void				update(Time dt) final;
	bool						handleEvent(Event e);
	void						setID(int id);
	int							getID() const;
	void						addComponent(Component::Ptr component);
	Transform					getWorldTransform() const;
	FloatRect					getWorldBounds() const override;
	EntityContainer*			getEntityContainer() const;
	glm::ivec2					getSize() const override;
	AppContext					getContext() const;

	template<typename T>
	T*							getComponent() const;

private:
	virtual void				draw(const Window& window, RenderStates states) const final;

private:
	Entity*						mParent = nullptr;
	std::vector<Component::Ptr>	mComponents;
	int							mID = -1;
	EntityContainer*			mEntityContainer;
	AppContext					mContext;
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