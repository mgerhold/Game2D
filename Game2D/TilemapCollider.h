#pragma once

#include "Collider.h"
#include <vector>
#include "Rect.h"

class TilemapCollider : public Collider {
public:
	void recalculateBoxes();

private:
	void onAwake() override;

	std::optional<CollisionInfo>	checkCollision(Collider* other) override;
	std::vector<FloatRect> mBoxes;

	friend class BoxCollider;
};