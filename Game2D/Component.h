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
	const Entity* getEntity() const;
	template<typename T>
	T* getComponent() const;

private:
	virtual void draw(const Window& window, RenderStates states) const final;
	virtual void onAwake();
	virtual void onDraw(const Window& window, RenderStates states) const;
	virtual void onUpdate(Time dt);
	void setEntity(const Entity* entity);

private:
	const Entity* mEntity;

	friend class Entity;
};