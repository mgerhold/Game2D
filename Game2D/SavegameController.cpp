#include "SavegameController.h"
#include <iostream>
#include "Entity.h"
#include <fstream>
#include "TilemapCollider.h"

using namespace std::literals::string_literals;

SavegameController::SavegameController()
	: Component()
	, mTilemap(nullptr)
{}

void SavegameController::saveLevel(const std::string& filename) const {
	try {
		std::ofstream ofs(filename.c_str(), std::ofstream::out);
		auto width = mTilemap->getWidth();
		auto height = mTilemap->getHeight();
		ofs.write(reinterpret_cast<char*>(&width), sizeof(width));
		ofs.write(reinterpret_cast<char*>(&height), sizeof(height));

		for (int y = 0; y < mTilemap->getHeight(); y++) {
			for (int x = 0; x < mTilemap->getWidth(); x++) {
				auto tile = mTilemap->getTile(x, y);
				ofs.write(reinterpret_cast<char*>(&tile), sizeof(tile));
			}
		}

		ofs.close();
	} catch (const std::exception& e) {
		throw std::runtime_error(("Error while saving level file "s + filename + ": "s + std::string(e.what())).c_str());
	}
}

void SavegameController::loadLevel(const std::string& filename) {
	try {
		std::ifstream ifs(filename.c_str(), std::ifstream::in);

		decltype(mTilemap->getWidth()) width;
		decltype(mTilemap->getHeight()) height;
		ifs.read(reinterpret_cast<char*>(&width), sizeof(width));
		ifs.read(reinterpret_cast<char*>(&height), sizeof(height));

		mTilemap->resize(width, height);

		for (int y = 0; y < mTilemap->getHeight(); y++) {
			for (int x = 0; x < mTilemap->getWidth(); x++) {
				Tile tile;
				ifs.read(reinterpret_cast<char*>(&tile), sizeof(tile));
				mTilemap->setTile(x, y, tile);
			}
		}


		ifs.close();
	} catch (const std::exception& e) {
		throw std::runtime_error(("Error while loading level file "s + filename + ": "s + std::string(e.what())).c_str());
	}

	auto tilemapCollider = getEntity()->getComponent<TilemapCollider>();
	if (tilemapCollider) {
		tilemapCollider->recalculateBoxes();
	}
}

void SavegameController::onAwake() {
	mTilemap = getEntity()->getComponent<Tilemap>();
}

void SavegameController::onDraw(const Window& window, RenderStates states) const {}

void SavegameController::onUpdate(Time dt) {}
