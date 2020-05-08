#pragma once

#include "Drawable.h"
#include <memory>
#include "Time.h"

class Entity;

class Component : public Drawable {
public:
	using Ptr = std::unique_ptr<Component>;

public:
	Component();
	void update(Time dt);
	void awake();

protected:
	Entity* getEntity();
	/*template<typename T>
	T* getComponent() const;*/

private:
	virtual void draw(const Window& window, RenderStates states) const final;
	virtual void onAwake();
	virtual void onDraw(const Window& window, RenderStates states) const;
	virtual void onUpdate(Time dt);
	void setEntity(Entity* entity);

private:
	Entity* mEntity;

	friend class Entity;
};

/*template<typename T>
T* Component::getComponent() const {
	if (!mEntity)
		return nullptr;
	auto result = (*mEntity).template getComponent<T>();
	return result;
}*/