#include "SavegameController.h"
#include <iostream>
#include "Entity.h"
#include <fstream>

SavegameController::SavegameController()
	: Component()
	, mTilemap(nullptr)
{}

void SavegameController::saveLevel(const std::string& filename) const {
	std::ofstream ofs(filename.c_str(), std::ofstream::out);
	auto width = mTilemap->getWidth();
	auto height = mTilemap->getHeight();
	ofs.write(reinterpret_cast<char*>(&width), sizeof(width));
	ofs.write(reinterpret_cast<char*>(&height), sizeof(height));

	for (int y = 0; y < mTilemap->getHeight(); y++) {
		for (int x = 0; x < mTilemap->getWidth(); x++) {
			auto tile = mTilemap->getTile(x, y);
			ofs.write(reinterpret_cast<char*>(&tile.tilesetX), sizeof(tile.tilesetX));
			ofs.write(reinterpret_cast<char*>(&tile.tilesetY), sizeof(tile.tilesetY));
		}
	}

	ofs.close();
}

void SavegameController::loadLevel(const std::string& filename) {
	std::ifstream ifs(filename.c_str(), std::ifstream::in);

	decltype(mTilemap->getWidth()) width;
	decltype(mTilemap->getHeight()) height;
	ifs.read(reinterpret_cast<char*>(&width), sizeof(width));
	ifs.read(reinterpret_cast<char*>(&height), sizeof(height));

	mTilemap->resize(width, height);

	for (int y = 0; y < mTilemap->getHeight(); y++) {
		for (int x = 0; x < mTilemap->getWidth(); x++) {
			Tile tile;
			ifs.read(reinterpret_cast<char*>(&tile.tilesetX), sizeof(tile.tilesetX));
			ifs.read(reinterpret_cast<char*>(&tile.tilesetY), sizeof(tile.tilesetY));
			mTilemap->setTile(x, y, tile);
		}
	}

	std::cout << "Size of loaded map: " << width << ", " << height << "\n";

	ifs.close();
}

void SavegameController::onAwake() {
	mTilemap = getEntity()->getComponent<Tilemap>();
}

void SavegameController::onDraw(const Window& window, RenderStates states) const {}

void SavegameController::onUpdate(Time dt) {}
