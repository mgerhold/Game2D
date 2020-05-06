#include "Tilemap.h"
#include <iostream>

Tilemap::Tilemap(int width, int height, const Texture& tilemapTexture, int tileWidth, int tileHeight)
	: Component()
	, mWidth(width)
	, mHeight(height)
	, mTexture(&tilemapTexture)
	, mTileWidth(tileWidth)
	, mTileHeight(tileHeight)
	, mVertexArray(PrimitiveType::Quads, DrawType::Static)
{
	mTiles = std::vector<Tile>(static_cast<size_t>(width) * height);
	mTiles[0].id = 1;
	mTiles[1].id = 2;
	mTiles[2].id = 3;
	rebuildVertexArray();
}

int Tilemap::getNumTiles() const {
	return (mTexture->getSize().x / mTileWidth) * (mTexture->getSize().y / mTileHeight);
}

int Tilemap::getTileWidth() const {
	return mTileWidth;
}

int Tilemap::getTileHeight() const {
	return mTileHeight;
}

int Tilemap::getWidth() const {
	return mWidth;
}

int Tilemap::getHeight() const {
	return mHeight;
}

const Texture& Tilemap::getTexture() const {
	return *mTexture;
}

void Tilemap::setTile(int x, int y, Tile tile) {
	int n = x + y * mWidth;
	if (n < 0 || n >= mWidth * mHeight)
		return;
	mTiles[n] = tile;
	updateVertexArrayAt(x, y);
}

void Tilemap::onAwake() {

}

void Tilemap::onDraw(const Window& window, RenderStates states) const {
	states.texture = mTexture;
	window.draw(mVertexArray, states);
}

void Tilemap::onUpdate(Time dt) {

}

void Tilemap::rebuildVertexArray() {
	/*int height = mTexture->getSize().y / mTileHeight;
	float uSize = static_cast<float>(mTileWidth) / mTexture->getSize().x;
	float vSize = static_cast<float>(mTileHeight) / mTexture->getSize().y;
	int tilesPerRow = mTexture->getSize().x / mTileWidth;*/
	mVertexArray.clear();
	mVertexArray.resize(static_cast<size_t>(mWidth) * mHeight * 4);
	for (int x = 0; x < mWidth; x++) {
		for (int y = 0; y < mHeight; y++) {	
			/*Tile tile = mTiles[static_cast<size_t>(x) + static_cast<size_t>(y) * mWidth];
			int posX = tile.id % tilesPerRow;
			int posY = tile.id / tilesPerRow;
			posY = height - posY - 1;
			mVertexArray.append(Vertex(glm::vec2((x + 0) * mTileWidth, (y + 0) * mTileWidth), glm::vec2((posX + 0) * uSize, (posY + 0) * vSize)));
			mVertexArray.append(Vertex(glm::vec2((x + 1) * mTileWidth, (y + 0) * mTileWidth), glm::vec2((posX + 1) * uSize, (posY + 0) * vSize)));
			mVertexArray.append(Vertex(glm::vec2((x + 1) * mTileWidth, (y + 1) * mTileWidth), glm::vec2((posX + 1) * uSize, (posY + 1) * vSize)));
			mVertexArray.append(Vertex(glm::vec2((x + 0) * mTileWidth, (y + 1) * mTileWidth), glm::vec2((posX + 0) * uSize, (posY + 1) * vSize)));*/
			updateVertexArrayAt(x, y);
		}
	}
}

void Tilemap::updateVertexArrayAt(int x, int y) {
	if (x < 0 || x >= mWidth || y < 0 || y > mHeight)
		return;

	int height = mTexture->getSize().y / mTileHeight;
	float uSize = static_cast<float>(mTileWidth) / mTexture->getSize().x;
	float vSize = static_cast<float>(mTileHeight) / mTexture->getSize().y;
	int tilesPerRow = mTexture->getSize().x / mTileWidth;

	Tile tile = mTiles[static_cast<size_t>(x) + static_cast<size_t>(y) * mWidth];
	int posX = tile.id % tilesPerRow;
	int posY = tile.id / tilesPerRow;
	posY = height - posY - 1;
	int index = (x + y * mWidth) * 4;
	mVertexArray.modify(static_cast<size_t>(index) + 0, Vertex(glm::vec2((x + 0) * mTileWidth, (y + 0) * mTileWidth), glm::vec2((posX + 0) * uSize, (posY + 0) * vSize)));
	mVertexArray.modify(static_cast<size_t>(index) + 1, Vertex(glm::vec2((x + 1) * mTileWidth, (y + 0) * mTileWidth), glm::vec2((posX + 1) * uSize, (posY + 0) * vSize)));
	mVertexArray.modify(static_cast<size_t>(index) + 2, Vertex(glm::vec2((x + 1) * mTileWidth, (y + 1) * mTileWidth), glm::vec2((posX + 1) * uSize, (posY + 1) * vSize)));
	mVertexArray.modify(static_cast<size_t>(index) + 3, Vertex(glm::vec2((x + 0) * mTileWidth, (y + 1) * mTileWidth), glm::vec2((posX + 0) * uSize, (posY + 1) * vSize)));
	/*mVertexArray.append(Vertex(glm::vec2((x + 0) * mTileWidth, (y + 0) * mTileWidth), glm::vec2((posX + 0) * uSize, (posY + 0) * vSize)));
	mVertexArray.append(Vertex(glm::vec2((x + 1) * mTileWidth, (y + 0) * mTileWidth), glm::vec2((posX + 1) * uSize, (posY + 0) * vSize)));
	mVertexArray.append(Vertex(glm::vec2((x + 1) * mTileWidth, (y + 1) * mTileWidth), glm::vec2((posX + 1) * uSize, (posY + 1) * vSize)));
	mVertexArray.append(Vertex(glm::vec2((x + 0) * mTileWidth, (y + 1) * mTileWidth), glm::vec2((posX + 0) * uSize, (posY + 1) * vSize)));*/
}
