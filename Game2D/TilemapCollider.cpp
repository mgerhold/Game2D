#include "TilemapCollider.h"
#include "BoxCollider.h"
#include "TilesetInfo.h"
#include "Tilemap.h"
#include <iostream>

void TilemapCollider::onAwake() {
	Collider::onAwake();
	recalculateBoxes();
}

void TilemapCollider::recalculateBoxes() {
	const auto tilesetInfo = TilesetInfo::get();
	auto tilemap = getEntity()->getComponent<Tilemap>();

	std::vector<std::vector<glm::ivec2>> boxes;

	for (int x = 0; x < tilemap->getWidth(); x++) {
		std::vector<glm::ivec2> currentBox;
		for (int y = 0; y < tilemap->getHeight(); y++) {
			auto tile = tilemap->getTile(x, y);
			if (tilesetInfo.at(glm::ivec2(tile.tilesetX, tile.tilesetY)).isSolid) { // solid tile found
				currentBox.push_back(glm::ivec2(x, y));
			} else if (currentBox.size() > 0) { // flush
				boxes.push_back(currentBox);
				currentBox.clear();
			}
		}
		if (currentBox.size() > 0) { // flush
			boxes.push_back(currentBox);
			currentBox.clear();
		}
	}

	std::vector<std::vector<glm::ivec2>> reducedBoxes;
	std::vector<const std::vector<glm::ivec2>*> ignoreList;
	for (const auto& a : boxes) {
		if (std::find(ignoreList.begin(), ignoreList.end(), &a) != ignoreList.end())
			continue;

		std::vector<glm::ivec2> box = a;
		for (int x = a[0].x + 1; x < tilemap->getWidth(); x++) {
			bool merged = false;
			for (const auto& b : boxes) {
				if (&a != &b && a.size() == b.size()) {
					bool equal = true;
					for (size_t i = 0; i < a.size(); i++) {
						if (b[i].x != x || a[i].y != b[i].y) {
							equal = false;
							break;
						}
					}
					if (equal) { // should merge now
						box.insert(box.end(), b.begin(), b.end());
						merged = true;
						ignoreList.push_back(&b);
					}
				}
			}
			if (!merged)
				break;
		}
		reducedBoxes.push_back(box);
	}

	mBoxes.clear();
	for (const auto& box : reducedBoxes) {
		auto rect = FloatRect(
			static_cast<float>(box.front().x * tilemap->getTileWidth()),
			static_cast<float>(box.front().y * tilemap->getTileHeight()),
			static_cast<float>((box.back().x + 1) * tilemap->getTileWidth()),
			static_cast<float>((box.back().y + 1) * tilemap->getTileHeight())
		);
		mBoxes.push_back(rect);
	}
}

std::optional<CollisionInfo> TilemapCollider::checkCollision(Collider* other) {
	std::vector<CollisionInfo> collisions;
	auto otherBoxCollider = dynamic_cast<BoxCollider*>(other);
	if (otherBoxCollider) {
		for (const auto& box : mBoxes) {
			auto b = otherBoxCollider->getEntity()->getWorldBounds();
			if (box.intersects(b)) {
				auto intersection = box.getIntersection(b);
				CollisionInfo collisionInfo{
					.intersection = intersection,
					.penetration = glm::vec2(intersection.right - intersection.left, intersection.top - intersection.bottom),
					.thisBounds = box,
					.otherBounds = b,
					.other = otherBoxCollider->getEntity()
				};
				collisions.push_back(collisionInfo);
			}
		}
		if (collisions.size() > 0) {
			float maxPenetration = 0.f;
			const CollisionInfo* ptr = nullptr;
			for (const auto& c : collisions) {
				float area = c.penetration.x * c.penetration.y;
				if (area > maxPenetration) {
					maxPenetration = area;
					ptr = &c;
				}
			}
			if (ptr)
				return *ptr;			
		}
		return {};
	}
	return {};
}
