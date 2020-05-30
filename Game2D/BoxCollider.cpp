#include "BoxCollider.h"
#include "TilemapCollider.h"
#include <iostream>

std::optional<CollisionInfo> BoxCollider::checkCollision(Collider* other) {
	auto otherBoxCollider = dynamic_cast<BoxCollider*>(other);

	if (otherBoxCollider) {
		auto a = getEntity()->getWorldBounds();
		auto b = otherBoxCollider->getEntity()->getWorldBounds();
		if (a.intersects(b)) {
			CollisionInfo collisionInfo;
			collisionInfo.intersection = FloatRect(
				std::max(a.left, b.left),
				std::max(a.bottom, b.bottom),
				std::min(a.right, b.right),
				std::min(a.top, b.top)
			);
			collisionInfo.penetration = glm::vec2(
				collisionInfo.intersection.right - collisionInfo.intersection.left,
				collisionInfo.intersection.top - collisionInfo.intersection.bottom
			);
			collisionInfo.other = otherBoxCollider->getEntity();
			collisionInfo.thisBounds = a;
			collisionInfo.otherBounds = b;
			return collisionInfo;
		}
		return {};
	}

	auto otherTilemapCollider = dynamic_cast<TilemapCollider*>(other);
	if (otherTilemapCollider) {
		auto collision = otherTilemapCollider->checkCollision(this);
		if (collision) {
			std::swap(collision->thisBounds, collision->otherBounds);
			collision->other = other->getEntity();
		}
		return collision;
	}

	return {};
}
