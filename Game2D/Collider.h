#pragma once

#include "Component.h"
#include <optional>
#include <functional>
#include <set>
#include <vector>
#include "CollisionInfo.h"
#include "Rect.h"
#include "Entity.h"

class EntityContainer;
class Entity;

class Collider : public Component {
public:
	using CallbackFunc = std::function<void(CollisionInfo)>;
	enum class CallbackType {
		Enter,
		Colliding,
		Leave,
	};

public:
	void									registerCallback(CallbackType type, CallbackFunc func);
	FloatRect								getWorldBounds() const;

protected:
	virtual void							onAwake() override;
	virtual void							onUpdate(Time dt) override;

private:
	virtual std::optional<CollisionInfo>	checkCollision(Collider* other) = 0;

private:
	std::vector<CallbackFunc>				mOnCollisionEnterCallbacks;
	std::vector<CallbackFunc>				mOnCollidingCallbacks;
	std::vector<CallbackFunc>				mOnCollisionLeaveCallbacks;
	const EntityContainer*					mEntityContainer = nullptr;
	Entity*									mEntity = nullptr;
	std::set<Entity*>						mActiveCollisions;
	std::map<Entity*, CollisionInfo>		mActiveCollisionsInfos;
};