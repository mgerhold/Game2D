#pragma once

#include "Collider.h"

class BoxCollider : public Collider {
private:
	std::optional<CollisionInfo> checkCollision(Collider* other) override;
};