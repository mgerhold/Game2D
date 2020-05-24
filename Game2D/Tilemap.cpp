#include "Tilemap.h"
#include "TilemapCollider.h"
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
	resize(width, height);
	rebuildVertexArray();
}

void Tilemap::resize(int width, int height) {
	if (mTiles.size() != static_cast<size_t>(width) * height)
		mTiles = std::vector<Tile>(static_cast<size_t>(width) * height);
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

Tile Tilemap::getTile(int x, int y) const {
	return mTiles[static_cast<size_t>(x) + static_cast<size_t>(y) * mWidth];
}

int Tilemap::getTilemapTilesPerRow() const {
	return mTexture->getSize().x / mTileWidth;
}

int Tilemap::getTilemapTilesPerColumn() const {
	return mTexture->getSize().y / mTileHeight;
}

void Tilemap::fill(const Tile& tile) {
	for (int x = 0; x < mWidth; x++) {
		for (int y = 0; y < mHeight; y++) {
			setTile(x, y, tile);
		}
	}
	auto collider = getEntity()->getComponent<TilemapCollider>();
	if (collider)
		collider->recalculateBoxes();
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
	mVertexArray.clear();
	mVertexArray.resize(static_cast<size_t>(mWidth) * mHeight * 4);
	for (int x = 0; x < mWidth; x++) {
		for (int y = 0; y < mHeight; y++) {	
			updateVertexArrayAt(x, y);
		}
	}
}

void Tilemap::updateVertexArrayAt(int x, int y) {
	if (x < 0 || x >= mWidth || y < 0 || y > mHeight)
		return;

	const int height = mTexture->getSize().y / mTileHeight;
	const float uSize = static_cast<float>(mTileWidth) / mTexture->getSize().x;
	const float vSize = static_cast<float>(mTileHeight) / mTexture->getSize().y;

	const Tile tile = mTiles[static_cast<size_t>(x) + static_cast<size_t>(y) * mWidth];
	const int offsetX = mTileWidth * tile.tilesetX;
	const int offsetY = mTileHeight * (height - tile.tilesetY - 1);
	const int texWidth = mTexture->getSize().x;
	const int texHeight = mTexture->getSize().y;


	glm::vec2 uvs[4];
	uvs[0] = glm::vec2(static_cast<float>(offsetX) / (texWidth - 1), static_cast<float>(offsetY) / (texHeight - 1));
	uvs[1] = glm::vec2(static_cast<float>(offsetX + mTileWidth) / (texWidth - 1), static_cast<float>(offsetY) / (texHeight - 1));
	uvs[2] = glm::vec2(static_cast<float>(offsetX + mTileWidth) / (texWidth - 1), static_cast<float>(offsetY + mTileHeight - 1) / (texHeight - 1));
	uvs[3] = glm::vec2(static_cast<float>(offsetX) / (texWidth - 1), static_cast<float>(offsetY + mTileHeight - 1) / (texHeight - 1));

	const int index = (x + y * mWidth) * 4;
	mVertexArray.modify(static_cast<size_t>(index) + 0, Vertex(glm::vec2((x + 0) * mTileWidth, (y + 0) * mTileWidth), uvs[0]));
	mVertexArray.modify(static_cast<size_t>(index) + 1, Vertex(glm::vec2((x + 1) * mTileWidth, (y + 0) * mTileWidth), uvs[1]));
	mVertexArray.modify(static_cast<size_t>(index) + 2, Vertex(glm::vec2((x + 1) * mTileWidth, (y + 1) * mTileWidth), uvs[2]));
	mVertexArray.modify(static_cast<size_t>(index) + 3, Vertex(glm::vec2((x + 0) * mTileWidth, (y + 1) * mTileWidth), uvs[3]));
}
