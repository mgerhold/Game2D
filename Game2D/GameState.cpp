#include "GameState.h"
#include "Tilemap.h"
#include <iostream>
#include "SpriteRenderer.h"
#include "Button.h"
#include "SavegameController.h"

GameState::GameState(StateStack* stateStack)
	: State(stateStack)
	, mSelectionEntity(nullptr)
	, mLevelEntity(nullptr)
{
	getContext().textureHolder.load(TextureID::Tilemap, "textures/tileset.png");
	getContext().textureHolder.get(TextureID::Tilemap).setTextureFiltering(Texture::Filtering::Nearest);
	getContext().textureHolder.load(TextureID::TileSelection, "textures/selection.png");
	getContext().textureHolder.get(TextureID::TileSelection).setTextureFiltering(Texture::Filtering::Nearest);
	getContext().textureHolder.load(TextureID::ArrowLeft1, "textures/arrow_left1.png");
	getContext().textureHolder.get(TextureID::ArrowLeft1).setTextureFiltering(Texture::Filtering::Nearest);
	getContext().textureHolder.load(TextureID::ArrowLeft2, "textures/arrow_left2.png");
	getContext().textureHolder.get(TextureID::ArrowLeft2).setTextureFiltering(Texture::Filtering::Nearest);
	getContext().textureHolder.load(TextureID::ArrowRight1, "textures/arrow_right1.png");
	getContext().textureHolder.get(TextureID::ArrowRight1).setTextureFiltering(Texture::Filtering::Nearest);
	getContext().textureHolder.load(TextureID::ArrowRight2, "textures/arrow_right2.png");
	getContext().textureHolder.get(TextureID::ArrowRight2).setTextureFiltering(Texture::Filtering::Nearest);
	getContext().textureHolder.load(TextureID::Button2Normal, "textures/button2_normal.png");
	getContext().textureHolder.get(TextureID::Button2Normal).setTextureFiltering(Texture::Filtering::Nearest);
	getContext().textureHolder.load(TextureID::Button2Selected, "textures/button2_selected.png");
	getContext().textureHolder.get(TextureID::Button2Selected).setTextureFiltering(Texture::Filtering::Nearest);
	getContext().textureHolder.load(TextureID::Button2Active, "textures/button2_active.png");
	getContext().textureHolder.get(TextureID::Button2Active).setTextureFiltering(Texture::Filtering::Nearest);

	// level entity
	auto levelEntity = std::make_unique<Entity>();
	// tilemap component
		// TODO: Remove magic numbers
	auto tilemap = std::make_unique<Tilemap>(100, 30, getContext().textureHolder.get(TextureID::Tilemap), 16, 16);
	std::cout << "Number of tiles in tilemap: " << tilemap->getNumTiles() << "\n";
	levelEntity->addComponent(std::move(tilemap));
	mLevelEntity = levelEntity.get();
	// savegame controller component
	auto savegameController = std::make_unique<SavegameController>();
	mLevelEntity->addComponent(std::move(savegameController));
	// add level entity to entity container
	mEntityContainer.add(std::move(levelEntity));

	// tilemap selection
	auto selectionEntity = std::make_unique<Entity>();
	Sprite selectionSprite;
	selectionSprite.setTexture(getContext().textureHolder.get(TextureID::TileSelection));
	auto selectionSpriteRenderer = std::make_unique<SpriteRenderer>(selectionSprite);
	selectionEntity->addComponent(std::move(selectionSpriteRenderer));
	mSelectionEntity = selectionEntity.get();
	mEntityContainer.add(std::move(selectionEntity));

	// buttons
		// prev button
	auto prevButton = std::make_shared<GUI::Button>();
	prevButton->setNormalTexture(getContext().textureHolder.get(TextureID::ArrowLeft1));
	prevButton->setSelectedTexture(getContext().textureHolder.get(TextureID::ArrowLeft1));
	prevButton->setActiveTexture(getContext().textureHolder.get(TextureID::ArrowLeft2));
	prevButton->setPosition(-400.f, 300.f);
	prevButton->setCallbackFunc([this]() {
		decSelection();
	});
	mGUIContainer.pack(prevButton);
		// next button
	auto nextButton = std::make_shared<GUI::Button>();
	nextButton->setNormalTexture(getContext().textureHolder.get(TextureID::ArrowRight1));
	nextButton->setSelectedTexture(getContext().textureHolder.get(TextureID::ArrowRight1));
	nextButton->setActiveTexture(getContext().textureHolder.get(TextureID::ArrowRight2));
	nextButton->setPosition(-250.f, 300.f);
	nextButton->setCallbackFunc([this]() {
		incSelection();
	});
	mGUIContainer.pack(nextButton);
		// save button
	auto saveButton = std::make_shared<GUI::Button>();
	saveButton->setNormalTexture(getContext().textureHolder.get(TextureID::Button2Normal));
	saveButton->setSelectedTexture(getContext().textureHolder.get(TextureID::Button2Selected));
	saveButton->setActiveTexture(getContext().textureHolder.get(TextureID::Button2Active));
	saveButton->setPosition(-400.f, 200.f);
	saveButton->setFont(getContext().fontHolder.get(FontID::Default), 30);
	saveButton->setFontColor(Color::Black);
	saveButton->setString("Save");
	saveButton->setCallbackFunc([this]() {
		mLevelEntity->getComponent<SavegameController>()->saveLevel("level01.lvl");
	});
	mGUIContainer.pack(saveButton);
		// load button
	auto loadButton = std::make_shared<GUI::Button>();
	loadButton->setNormalTexture(getContext().textureHolder.get(TextureID::Button2Normal));
	loadButton->setSelectedTexture(getContext().textureHolder.get(TextureID::Button2Selected));
	loadButton->setActiveTexture(getContext().textureHolder.get(TextureID::Button2Active));
	loadButton->setPosition(-400.f, 100.f);
	loadButton->setFont(getContext().fontHolder.get(FontID::Default), 30);
	loadButton->setFontColor(Color::Black);
	loadButton->setString("Load");
	loadButton->setCallbackFunc([this]() {
		mLevelEntity->getComponent<SavegameController>()->loadLevel("level01.lvl");
		});
	mGUIContainer.pack(loadButton);
	
	// preview tile
	mPreviewSprite.setTexture(getContext().textureHolder.get(TextureID::Tilemap));
	updateSelectedPreviewTile();
	mPreviewSprite.setPosition(-350.f, 300.f);
	mPreviewSprite.setScale(glm::vec2(4.f));

	// camera settings
	mCamera.setZoom(5.f);

	// awake entities
	mEntityContainer.awake();
}

GameState::~GameState() {
	getContext().textureHolder.unload(TextureID::Tilemap);
	getContext().textureHolder.unload(TextureID::TileSelection);
	getContext().textureHolder.unload(TextureID::ArrowLeft1);
	getContext().textureHolder.unload(TextureID::ArrowLeft2);
	getContext().textureHolder.unload(TextureID::ArrowRight1);
	getContext().textureHolder.unload(TextureID::ArrowRight2);
	getContext().textureHolder.unload(TextureID::Button2Normal);
	getContext().textureHolder.unload(TextureID::Button2Selected);
	getContext().textureHolder.unload(TextureID::Button2Active);
}

bool GameState::update(Time dt) {
	const auto& window = getContext().window;

	mEntityContainer.update(dt);
	constexpr float movementSpeed = 200.f;
	if (window.isKeyPressed(Key::D))
		mCamera.move(glm::vec2(movementSpeed, 0) * dt.asSeconds());
	if (window.isKeyPressed(Key::A))
		mCamera.move(glm::vec2(-movementSpeed, 0) * dt.asSeconds());
	if (window.isKeyPressed(Key::W))
		mCamera.move(glm::vec2(0, movementSpeed) * dt.asSeconds());
	if (window.isKeyPressed(Key::S))
		mCamera.move(glm::vec2(0, -movementSpeed) * dt.asSeconds());

	auto* tilemap = mLevelEntity->getComponent<Tilemap>();
	auto mousePos = window.windowToWorldCoords(window.getMousePosition(), mCamera);
	auto tilePos = glm::vec2(
		mousePos.x / tilemap->getTileWidth(),
		mousePos.y / tilemap->getTileHeight()
	);
	auto& sprite = mSelectionEntity->getComponent<SpriteRenderer>()->getSprite();
	if (tilePos.x >= 0 && tilePos.x < tilemap->getWidth() && tilePos.y >= 0 && tilePos.y < tilemap->getHeight()) {
		sprite.setPosition(
			std::trunc(tilePos.x) * tilemap->getTileWidth(),
			std::trunc(tilePos.y) * tilemap->getTileHeight()
		);
	}

	return true;
}

void GameState::draw(const Window& window) const {
	window.draw(mEntityContainer, mCamera);
	window.draw(mGUIContainer);
	window.draw(mPreviewSprite);
}

bool GameState::handleEvent(Event e) {
	if (!mGUIContainer.handleEvent(e, getContext().window)) {
		switch (e.type) {
			case Event::Type::MouseScroll:
				mCamera.zoom(1.f + 0.1f * static_cast<float>(e.mouseScrollDelta.y));
				break;
			case Event::Type::MouseButtonPress: {
				// determine which tile was clicked
				auto mousePos = getContext().window.windowToWorldCoords(glm::vec2(e.mousePosition.x, e.mousePosition.y), mCamera);
				auto* tilemap = mLevelEntity->getComponent<Tilemap>();
				auto tilePos = glm::ivec2(
					std::trunc(mousePos.x / tilemap->getTileWidth()),
					std::trunc(mousePos.y / tilemap->getTileHeight())
				);
				if (tilePos.x < 0 || tilePos.y < 0 || tilePos.x >= tilemap->getWidth() || tilePos.y >= tilemap->getHeight())
					break;
				switch (e.mouseButton) {
					case MouseButton::Left: {
						Tile tile;
						tile.tilesetX = mSelectedPreviewTile % tilemap->getTilemapTilesPerRow();
						tile.tilesetY = mSelectedPreviewTile / tilemap->getTilemapTilesPerRow();
						tilemap->setTile(tilePos.x, tilePos.y, tile);
						break;
					}
					case MouseButton::Right: {
						Tile tile = tilemap->getTile(tilePos.x, tilePos.y);
						setSelection(tile.tilesetX + tile.tilesetY * tilemap->getTilemapTilesPerRow());
						break;
					}
				}				
				break;
			}
		}
	}
	return false;
}

void GameState::updateSelectedPreviewTile() {
	auto tilemap = mLevelEntity->getComponent<Tilemap>();
	const auto& texture = tilemap->getTexture();
	int xPos = mSelectedPreviewTile % (texture.getSize().x / tilemap->getTileWidth());
	int yPos = mSelectedPreviewTile / (texture.getSize().x / tilemap->getTileWidth());
	const int sizeY = texture.getSize().y / tilemap->getTileHeight();
	yPos = sizeY - yPos - 1;
	mPreviewSprite.setTextureRect(IntRect(
		xPos * tilemap->getTileWidth(),
		yPos * tilemap->getTileHeight(),
		(xPos + 1) * tilemap->getTileWidth() - 1,
		(yPos + 1) * tilemap->getTileHeight() - 1
	));
}

void GameState::setSelection(int n) {
	auto tilemap = mLevelEntity->getComponent<Tilemap>();
	const auto& texture = tilemap->getTexture();
	const int sizeX = texture.getSize().x / tilemap->getTileWidth();
	const int sizeY = texture.getSize().y / tilemap->getTileHeight();

	if (n < 0)
		n = 0;
	if (n >= sizeX * sizeY)
		n = sizeX * sizeY - 1;
	mSelectedPreviewTile = n;
	updateSelectedPreviewTile();
}

void GameState::incSelection() {
	setSelection(mSelectedPreviewTile + 1);
}

void GameState::decSelection() {
	setSelection(mSelectedPreviewTile - 1);
}
