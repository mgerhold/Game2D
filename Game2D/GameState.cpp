#include "GameState.h"
#include "Tilemap.h"
#include <iostream>
#include "SpriteRenderer.h"
#include "Button.h"

GameState::GameState(StateStack* stateStack)
	: State(stateStack)
	, mSelectionEntity(nullptr)
	, mTilemapEntity(nullptr)
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

	// tilemap
	auto mapEntity = std::make_unique<Entity>();
	// TODO: Remove magic numbers
	auto tilemap = std::make_unique<Tilemap>(100, 30, getContext().textureHolder.get(TextureID::Tilemap), 16, 16);
	std::cout << "Number of tiles in tilemap: " << tilemap->getNumTiles() << "\n";
	mapEntity->addComponent(std::move(tilemap));
	mTilemapEntity = mapEntity.get();
	mEntityContainer.add(std::move(mapEntity));

	// tilemap selection
	auto selectionEntity = std::make_unique<Entity>();
	Sprite selectionSprite;
	selectionSprite.setTexture(getContext().textureHolder.get(TextureID::TileSelection));
	auto selectionSpriteRenderer = std::make_unique<SpriteRenderer>(selectionSprite);
	selectionEntity->addComponent(std::move(selectionSpriteRenderer));
	mSelectionEntity = selectionEntity.get();
	mEntityContainer.add(std::move(selectionEntity));

	// buttons
	auto prevButton = std::make_shared<GUI::Button>();
	prevButton->setNormalTexture(getContext().textureHolder.get(TextureID::ArrowLeft1));
	prevButton->setSelectedTexture(getContext().textureHolder.get(TextureID::ArrowLeft1));
	prevButton->setActiveTexture(getContext().textureHolder.get(TextureID::ArrowLeft2));
	prevButton->setPosition(-400.f, 300.f);
	prevButton->setCallbackFunc([this]() {
		decSelection();
		updateSelectedPreviewTile();
	});
	mGUIContainer.pack(prevButton);
	auto nextButton = std::make_shared<GUI::Button>();
	nextButton->setNormalTexture(getContext().textureHolder.get(TextureID::ArrowRight1));
	nextButton->setSelectedTexture(getContext().textureHolder.get(TextureID::ArrowRight1));
	nextButton->setActiveTexture(getContext().textureHolder.get(TextureID::ArrowRight2));
	nextButton->setPosition(-250.f, 300.f);
	nextButton->setCallbackFunc([this]() {
		incSelection();
		updateSelectedPreviewTile();
	});
	mGUIContainer.pack(nextButton);
	
	// preview tile
	mPreviewSprite.setTexture(getContext().textureHolder.get(TextureID::Tilemap));
	updateSelectedPreviewTile();
	mPreviewSprite.setPosition(-350.f, 300.f);
	mPreviewSprite.setScale(glm::vec2(4.f));

	// camera settings
	mCamera.setZoom(5.f);
}

GameState::~GameState() {
	getContext().textureHolder.unload(TextureID::Tilemap);
	getContext().textureHolder.unload(TextureID::TileSelection);
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

	auto* tilemap = mTilemapEntity->getComponent<Tilemap>();
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
		case Event::Type::MouseButtonPress:
			switch (e.mouseButton) {
			case MouseButton::Left:
				auto mousePos = getContext().window.windowToWorldCoords(/*getContext().window.getMousePosition()*/ glm::vec2(e.mousePosition.x, e.mousePosition.y), mCamera);
				auto* tilemap = mTilemapEntity->getComponent<Tilemap>();
				auto tilePos = glm::ivec2(
					std::trunc(mousePos.x / tilemap->getTileWidth()),
					std::trunc(mousePos.y / tilemap->getTileHeight())
				);
				Tile tile;
				tile.id = mSelectedPreviewTile;
				tilemap->setTile(tilePos.x, tilePos.y, tile);
				break;
			}
			break;
		}
	}
	return false;
}

void GameState::updateSelectedPreviewTile() {
	auto tilemap = mTilemapEntity->getComponent<Tilemap>();
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
	auto tilemap = mTilemapEntity->getComponent<Tilemap>();
	const auto& texture = tilemap->getTexture();
	const int sizeX = texture.getSize().x / tilemap->getTileWidth();
	const int sizeY = texture.getSize().y / tilemap->getTileHeight();

	if (n < 0)
		n = 0;
	if (n >= sizeX * sizeY)
		n = sizeX * sizeY - 1;
	mSelectedPreviewTile = n;
	/*int x = n % (texture.getSize().x / tilemap->getTileWidth());
	int y = n / (texture.getSize().x / tilemap->getTileWidth());
	if (x >= sizeX)
		x = sizeX - 1;
	if (y >= sizeY)
		y = sizeY - 1;*/
}

void GameState::incSelection() {
	setSelection(mSelectedPreviewTile + 1);
}

void GameState::decSelection() {
	setSelection(mSelectedPreviewTile - 1);
}
