#pragma once

#include "Entity.h"

class EntityContainer : public Drawable {
public:
	void	add(Entity::Ptr entity);
	void	awake();
	void	update(Time dt);

private:
	virtual void	draw(const Window& window, RenderStates states) const;
	std::vector<Entity::Ptr>	mEntities;
};