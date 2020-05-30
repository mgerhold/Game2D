#include "TilesetInfo.h"

std::unordered_map<glm::ivec2, TileInfo> TilesetInfo::get() {
	return {
		{ glm::ivec2(0, 0), TileInfo{.isSolid = true } },
		{ glm::ivec2(1, 0), TileInfo{.isSolid = true } },
		{ glm::ivec2(2, 0), TileInfo{.isSolid = true } },
		{ glm::ivec2(3, 0), TileInfo{.isSolid = true } },
		{ glm::ivec2(4, 0), TileInfo{.isSolid = true } },
													 
		{ glm::ivec2(0, 1), TileInfo{.isSolid = true } },
		{ glm::ivec2(1, 1), TileInfo{.isSolid = true } },
		{ glm::ivec2(2, 1), TileInfo{.isSolid = true } },
		{ glm::ivec2(3, 1), TileInfo{.isSolid = true } },
		{ glm::ivec2(4, 1), TileInfo{.isSolid = true } },
													 
		{ glm::ivec2(0, 2), TileInfo{.isSolid = true } },
		{ glm::ivec2(1, 2), TileInfo{.isSolid = true } },
		{ glm::ivec2(2, 2), TileInfo{.isSolid = true } },
		{ glm::ivec2(3, 2), TileInfo{.isSolid = true } },
		{ glm::ivec2(4, 2), TileInfo{.isSolid = true } },
													 
		{ glm::ivec2(0, 3), TileInfo{.isSolid = true } },
		{ glm::ivec2(1, 3), TileInfo{.isSolid = true } },
		{ glm::ivec2(2, 3), TileInfo{.isSolid = true } },
		{ glm::ivec2(3, 3), TileInfo{.isSolid = false } },
		{ glm::ivec2(4, 3), TileInfo{.isSolid = false } },
	};
}
