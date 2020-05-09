#include "Collider.h"
#include "EntityContainer.h"
#include <iostream>
#include <algorithm>
#include <map>

void Collider::onAwake() {
	mEntity = getEntity();
	mEntityContainer = mEntity->getEntityContainer();
}

void Collider::onUpdate(Time dt) {
	std::set<Entity*> collisions;
	std::map<Entity*, CollisionInfo> collisionInfos;
	// get all entities with Collider Component
	auto entities = mEntityContainer->getEntitiesWithComponent<Collider>();
	for (const auto& entity : entities) {
		if (entity != mEntity) { // only check collision against other entities
			auto collisionInfo = checkCollision(entity->getComponent<Collider>());
			if (collisionInfo) {
				// colliding -> invoke callback
				for (const auto& func : mOnCollidingCallbacks)
					func(*collisionInfo);
				collisions.insert(entity);
				collisionInfos[entity] = *collisionInfo;
				//std::cout << "Collision occured!\n";
			}
		}
	}
	std::set<Entity*> newCollisions;
	std::set_difference(
		collisions.begin(), collisions.end(),
		mActiveCollisions.begin(), mActiveCollisions.end(),
		std::inserter(newCollisions, newCollisions.end())
	);
	for (const auto& func : mOnCollisionEnterCallbacks) {
		for (const auto& entity : newCollisions) {
			func(collisionInfos[entity]);
		}
	}

	std::set<Entity*> endedCollisions;
	std::set_difference(
		mActiveCollisions.begin(), mActiveCollisions.end(),
		collisions.begin(), collisions.end(),
		std::inserter(endedCollisions, endedCollisions.end())
	);
	for (const auto& func : mOnCollisionLeaveCallbacks) {
		for (const auto& entity : endedCollisions) {
			//func(collisionInfos[entity]);
			func(mActiveCollisionsInfos[entity]);
		}
	}

	mActiveCollisions = collisions;
	mActiveCollisionsInfos = collisionInfos;
}

void Collider::registerCallback(CallbackType type, CallbackFunc func) {
	switch (type) {
		case CallbackType::Enter:
			mOnCollisionEnterCallbacks.push_back(func);
			break;
		case CallbackType::Colliding:
			mOnCollidingCallbacks.push_back(func);
			break;
		case CallbackType::Leave:
			mOnCollisionLeaveCallbacks.push_back(func);
			break;
	}
}

FloatRect Collider::getWorldBounds() const {
	return FloatRect();
}
