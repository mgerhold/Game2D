#pragma once

#include "Entity.h"
#include <vector>

class EntityContainer : public Drawable {
public:
	void	add(Entity::Ptr entity);
	void	awake();
	void	update(Time dt);

	template<typename T>
	std::vector<Entity*>	getEntitiesWithComponent() const;

private:
	virtual void	draw(const Window& window, RenderStates states) const;
	std::vector<Entity::Ptr>	mEntities;
};

template<typename T>
inline std::vector<Entity*> EntityContainer::getEntitiesWithComponent() const {
	std::vector<Entity*> result;
	for (const auto& entity : mEntities) {
		auto ptr = entity->getComponent<T>();
		if (ptr)
			result.push_back(entity.get());
	}
	return result;
}
