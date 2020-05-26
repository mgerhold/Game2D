#include "GameState.h"
#include "Tilemap.h"
#include <iostream>
#include "SpriteRenderer.h"
#include "Button.h"
#include "SavegameController.h"
#include "AnimationRenderer.h"
#include "RigidBody.h"
#include "BoxCollider.h"
#include "TilemapCollider.h"
#include "PlayerController.h"
#include "AnimationController.h"
#include "ParallaxController.h"
#include "TextRenderer.h"
#include <vector>
#include <utility> // std::pair

namespace {
	using namespace std::literals::string_literals;

	const std::vector<std::pair<TextureID, std::string>> neededTextures = {
		{ TextureID::Tileset, "textures/tileset_new.png"s },
		{ TextureID::TileSelection, "textures/selection.png"s },
		{ TextureID::ArrowLeft1, "textures/arrow_left1.png"s },
		{ TextureID::ArrowLeft2, "textures/arrow_left2.png"s },
		{ TextureID::ArrowRight1, "textures/arrow_right1.png"s },
		{ TextureID::ArrowRight2, "textures/arrow_right2.png"s },
		{ TextureID::Button2Normal, "textures/button2_normal.png"s },
		{ TextureID::Button2Selected, "textures/button2_selected.png"s },
		{ TextureID::Button2Active, "textures/button2_active.png"s },
		{ TextureID::PlayerIdle, "textures/catIdle.png"s },
		{ TextureID::PlayerIdleReversed, "textures/catIdle_reversed.png"s },
		{ TextureID::PlayerRun, "textures/catWalk.png"s },
		{ TextureID::PlayerRunReversed, "textures/catWalk_reversed.png"s },
		{ TextureID::PlayerJump, "textures/catJump.png"s },
		{ TextureID::PlayerJumpReversed, "textures/catJump_reversed.png"s },
		{ TextureID::Background, "textures/bg2.jpg"s },
		{ TextureID::HourglassRunning, "textures/hourglassAnimation.png"s },
		{ TextureID::HourglassTurning, "textures/hourglassTurnAnimation.png"s },
	};
}

GameState::GameState(StateStack* stateStack)
	: State(stateStack)
	, mEntityContainer(getContext())
	, mHourglass(getContext(), mEntityContainer)
{
	for (const auto& [id, filename] : neededTextures) {
		getContext().textureHolder.load(id, filename);
		getContext().textureHolder.get(id).setTextureFiltering(Texture::Filtering::Nearest);
	}

	getContext().textureHolder.get(TextureID::Background).setTextureFiltering(Texture::Filtering::Linear);
	getContext().textureHolder.get(TextureID::Tileset).setTextureWrap(false);

	// parallax scrolling background tile
	auto background = std::make_unique<Entity>(&mEntityContainer, getContext());
	Sprite backgroundSprite;
	getContext().textureHolder.get(TextureID::Background).setTextureWrap(true);
	backgroundSprite.setTexture(getContext().textureHolder.get(TextureID::Background));
	backgroundSprite.setTiling(20, 20);
	backgroundSprite.centerOrigin();
	//backgroundSprite.setScale(0.2f, 0.2f);
	auto backgroundSpriteRenderer = std::make_unique<SpriteRenderer>(backgroundSprite);
	background->addComponent(std::move(backgroundSpriteRenderer));
	auto backgroundParallaxController = std::make_unique<ParallaxController>(mCamera, glm::vec2(0.2f));
	background->addComponent(std::move(backgroundParallaxController));
	mEntityContainer.add(std::move(background));

	// level entity
	auto tilemap = std::make_unique<Entity>(&mEntityContainer, getContext());
	// tilemapComponent
		// TODO: Remove magic numbers
	auto tilemapComponent = std::make_unique<Tilemap>(100, 30, getContext().textureHolder.get(TextureID::Tileset), 180, 180);
	tilemap->addComponent(std::move(tilemapComponent));
	mTilemap = tilemap.get();
	// savegame controller component
	auto savegameController = std::make_unique<SavegameController>();
	mTilemap->addComponent(std::move(savegameController));
	// tilemap collider
	auto tilemapCollider = std::make_unique<TilemapCollider>();
	mTilemap->addComponent(std::move(tilemapCollider));
	// add level entity to entity container
	mEntityContainer.add(std::move(tilemap));

	// player
	auto player = std::make_unique<Entity>(&mEntityContainer, getContext());
	mPlayer = player.get();
	// player.AnimationRenderer
	auto animationRenderer = std::make_unique<AnimationRenderer>();
	mPlayer->addComponent(std::move(animationRenderer));
	// player.AnimationController
	auto animationController = std::make_unique<AnimationController>();
	mPlayer->addComponent(std::move(animationController));
	// player.BoxCollider
	auto boxCollider = std::make_unique<BoxCollider>();
	player->addComponent(std::move(boxCollider));
	// player.RigidBody
	auto rigidBody = std::make_unique<RigidBody>();
	rigidBody->setGravity(glm::vec2(0.f, -2000.f));
	rigidBody->setDrag(0.01f);
	player->addComponent(std::move(rigidBody));
	// player.PlayerController
	auto playerController = std::make_unique<PlayerController>();
	player->addComponent(std::move(playerController));
	//player->setPosition(20.f, 90.f);
	player->setPosition(550.f, 400.f);
	mEntityContainer.add(std::move(player));

	/**** GUI STUFF STARTS HERE ****/
	// hourglass
	Text hourglass;
	hourglass.setFont(getContext().fontHolder.get(FontID::Default), 48);
	hourglass.setString("Test");

	// tilemapComponent selection
	auto selectionEntity = std::make_unique<Entity>(&mEntityContainer, getContext());
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
		mTilemap->getComponent<SavegameController>()->saveLevel("level01.lvl");
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
		mTilemap->getComponent<SavegameController>()->loadLevel("level01.lvl");
	});
	mGUIContainer.pack(loadButton);
	
	// preview tile
	mPreviewSprite.setTexture(getContext().textureHolder.get(TextureID::Tileset));
	updateSelectedPreviewTile();
	mPreviewSprite.setPosition(-350.f, 300.f);
	mPreviewSprite.setScale(glm::vec2(0.5));

	// camera settings
	mCamera.setZoom(0.5f);

	// awake entities
	mEntityContainer.awake();
	mHourglass.awake();

	// load map
	{
		auto ptr = mTilemap->getComponent<SavegameController>();
		ptr->loadLevel("level01.lvl");
		//Tile tile{ .tilesetX = 4, .tilesetY = 3 };
		//mTilemap->getComponent<Tilemap>()->fill(tile);
	}
}

GameState::~GameState() {
	for (const auto& [id, filename] : neededTextures) {
		getContext().textureHolder.unload(id);
	}
}

bool GameState::update(Time dt) {
	const auto& window = getContext().window;

	mEntityContainer.update(dt);
	/*constexpr float movementSpeed = 200.f;
	if (window.isKeyPressed(Key::D))
		mCamera.move(glm::vec2(movementSpeed, 0) * dt.asSeconds());
	if (window.isKeyPressed(Key::A))
		mCamera.move(glm::vec2(-movementSpeed, 0) * dt.asSeconds());
	if (window.isKeyPressed(Key::W))
		mCamera.move(glm::vec2(0, movementSpeed) * dt.asSeconds());
	if (window.isKeyPressed(Key::S))
		mCamera.move(glm::vec2(0, -movementSpeed) * dt.asSeconds());*/

	auto* tilemap = mTilemap->getComponent<Tilemap>();
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

	mHourglass.update(dt);

	auto currentCameraPos = mCamera.getPosition();
	auto targetPosition = mPlayer->getPosition();
	auto lerpPosition = (targetPosition - currentCameraPos) * 0.99f * dt.asSeconds() + currentCameraPos;
	mCamera.setPosition(lerpPosition);

	return true;
}

void GameState::draw(const Window& window) const {
	window.draw(mEntityContainer, mCamera);
	window.draw(mGUIContainer);
	window.draw(mPreviewSprite);
	window.draw(mHourglass);
}

bool GameState::handleEvent(Event e) {
	if (!mGUIContainer.handleEvent(e, getContext().window) && !mEntityContainer.handleEvent(e)) {
		switch (e.type) {
			/*case Event::Type::KeyPress:
				if (e.key == Key::Space)
					mPlayer->getComponent<RigidBody>()->accelerate(glm::vec2(0.f, 300.f));
				break;*/
			case Event::Type::MouseScroll:
				mCamera.zoom(1.f + 0.1f * static_cast<float>(e.mouseScrollDelta.y));
				break;
			case Event::Type::MouseButtonPress: {
				// determine which tile was clicked
				auto mousePos = getContext().window.windowToWorldCoords(glm::vec2(e.mousePosition.x, e.mousePosition.y), mCamera);
				auto* tilemap = mTilemap->getComponent<Tilemap>();
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
						auto tilemapCollider = mTilemap->getComponent<TilemapCollider>();
						if (tilemapCollider)
							tilemapCollider->recalculateBoxes();
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
	auto tilemap = mTilemap->getComponent<Tilemap>();
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
	auto tilemap = mTilemap->getComponent<Tilemap>();
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
