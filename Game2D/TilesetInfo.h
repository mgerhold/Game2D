#pragma once

#include <unordered_map>
#include <glm/vec2.hpp>
#include "TileInfo.h"

namespace TilesetInfo {
	std::unordered_map<glm::ivec2, TileInfo> get();
}

namespace std {
	template <>
	struct hash<glm::ivec2> {
		std::size_t operator()(const glm::ivec2& k) const {
			return (std::hash<int>()(k.x) ^ std::hash<int>()(k.y));
		}
	};
}