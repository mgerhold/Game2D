#pragma once

#include "Entity.h"
#include <vector>
#include "Event.h"
#include "AppContext.h"

class EntityContainer : public Drawable {
public:
								EntityContainer(AppContext context);

	void						add(Entity::Ptr entity);
	void						awake();
	void						update(Time dt);
	bool						handleEvent(Event e);
	AppContext					getContext() const;

	template<typename T>
	std::vector<Entity*>		getEntitiesWithComponent() const;

private:
	virtual void				draw(const Window& window, RenderStates states) const;
	std::vector<Entity::Ptr>	mEntities;
	AppContext					mContext;
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
