#include "TilesetInfo.h"

std::unordered_map<glm::ivec2, TileInfo> TilesetInfo::get() {
	return {
		{ glm::ivec2(0, 0), TileInfo{.isSolid = false } },
		{ glm::ivec2(1, 0), TileInfo{.isSolid = true } },
		{ glm::ivec2(2, 0), TileInfo{.isSolid = true } },
		{ glm::ivec2(3, 0), TileInfo{.isSolid = true } },
		{ glm::ivec2(4, 0), TileInfo{.isSolid = true } },
		{ glm::ivec2(5, 0), TileInfo{.isSolid = true } },
		{ glm::ivec2(6, 0), TileInfo{.isSolid = true } },
		{ glm::ivec2(7, 0), TileInfo{.isSolid = true } },

		{ glm::ivec2(0, 1), TileInfo{.isSolid = false } },
		{ glm::ivec2(1, 1), TileInfo{.isSolid = true } },
		{ glm::ivec2(2, 1), TileInfo{.isSolid = true } },
		{ glm::ivec2(3, 1), TileInfo{.isSolid = true } },
		{ glm::ivec2(4, 1), TileInfo{.isSolid = true } },
		{ glm::ivec2(5, 1), TileInfo{.isSolid = true } },
		{ glm::ivec2(6, 1), TileInfo{.isSolid = true } },
		{ glm::ivec2(7, 1), TileInfo{.isSolid = true } },

		{ glm::ivec2(0, 2), TileInfo{.isSolid = false } },
		{ glm::ivec2(1, 2), TileInfo{.isSolid = true } },
		{ glm::ivec2(2, 2), TileInfo{.isSolid = true } },
		{ glm::ivec2(3, 2), TileInfo{.isSolid = true } },
		{ glm::ivec2(4, 2), TileInfo{.isSolid = true } },
		{ glm::ivec2(5, 2), TileInfo{.isSolid = true } },
		{ glm::ivec2(6, 2), TileInfo{.isSolid = true } },
		{ glm::ivec2(7, 2), TileInfo{.isSolid = true } },

		{ glm::ivec2(0, 3), TileInfo{.isSolid = false } },
		{ glm::ivec2(1, 3), TileInfo{.isSolid = false } },
		{ glm::ivec2(2, 3), TileInfo{.isSolid = false } },
		{ glm::ivec2(3, 3), TileInfo{.isSolid = false } },
		{ glm::ivec2(4, 3), TileInfo{.isSolid = false } },
		{ glm::ivec2(5, 3), TileInfo{.isSolid = false } },
		{ glm::ivec2(6, 3), TileInfo{.isSolid = false } },
		{ glm::ivec2(7, 3), TileInfo{.isSolid = false } },

		{ glm::ivec2(0, 4), TileInfo{.isSolid = false } },
		{ glm::ivec2(1, 4), TileInfo{.isSolid = false } },
		{ glm::ivec2(2, 4), TileInfo{.isSolid = false } },
		{ glm::ivec2(3, 4), TileInfo{.isSolid = false } },
		{ glm::ivec2(4, 4), TileInfo{.isSolid = false } },
		{ glm::ivec2(5, 4), TileInfo{.isSolid = false } },
		{ glm::ivec2(6, 4), TileInfo{.isSolid = false } },
		{ glm::ivec2(7, 4), TileInfo{.isSolid = false } },

		{ glm::ivec2(0, 5), TileInfo{.isSolid = false } },
		{ glm::ivec2(1, 5), TileInfo{.isSolid = false } },
		{ glm::ivec2(2, 5), TileInfo{.isSolid = false } },
		{ glm::ivec2(3, 5), TileInfo{.isSolid = false } },
		{ glm::ivec2(4, 5), TileInfo{.isSolid = false } },
		{ glm::ivec2(5, 5), TileInfo{.isSolid = false } },
		{ glm::ivec2(6, 5), TileInfo{.isSolid = false } },
		{ glm::ivec2(7, 5), TileInfo{.isSolid = false } },

		{ glm::ivec2(0, 6), TileInfo{.isSolid = false } },
		{ glm::ivec2(1, 6), TileInfo{.isSolid = false } },
		{ glm::ivec2(2, 6), TileInfo{.isSolid = false } },
		{ glm::ivec2(3, 6), TileInfo{.isSolid = false } },
		{ glm::ivec2(4, 6), TileInfo{.isSolid = false } },
		{ glm::ivec2(5, 6), TileInfo{.isSolid = false } },
		{ glm::ivec2(6, 6), TileInfo{.isSolid = false } },
		{ glm::ivec2(7, 6), TileInfo{.isSolid = false } },

		{ glm::ivec2(0, 7), TileInfo{.isSolid = false } },
		{ glm::ivec2(1, 7), TileInfo{.isSolid = false } },
		{ glm::ivec2(2, 7), TileInfo{.isSolid = false } },
		{ glm::ivec2(3, 7), TileInfo{.isSolid = false } },
		{ glm::ivec2(4, 7), TileInfo{.isSolid = false } },
		{ glm::ivec2(5, 7), TileInfo{.isSolid = false } },
		{ glm::ivec2(6, 7), TileInfo{.isSolid = false } },
		{ glm::ivec2(7, 7), TileInfo{.isSolid = false } },
	};
}