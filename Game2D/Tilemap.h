#pragma once

#include "Component.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Tile.h"
#include <vector>

class Tilemap : public Component {
public:
						Tilemap(int width, int height, const Texture& tilemapTexture, int tileWidth, int tileHeight);

	int					getNumTiles() const;
	int					getTileWidth() const;
	int					getTileHeight() const;
	int					getWidth() const;
	int					getHeight() const;
	const Texture&		getTexture() const;
	void				setTile(int x, int y, Tile tile);

private:
	virtual void		onAwake() override;
	virtual void		onDraw(const Window& window, RenderStates states) const override;
	virtual void		onUpdate(Time dt) override;
	void				rebuildVertexArray();
	void				updateVertexArrayAt(int x, int y);

private:
	int					mWidth;
	int					mHeight;
	const Texture*		mTexture;
	int					mTileWidth;
	int					mTileHeight;
	VertexArray			mVertexArray;
	std::vector<Tile>	mTiles;
};