#include <iostream>
#include "Application.h"
#include "ScopedProfiler.h"



int main() {
	try {
		Application app;
		app.run();
	} catch (const std::exception& e) {
		std::cout << "CAUGHT EXCEPTION: " << e.what() << "\n";
		std::cin.get();
	}
	ScopedProfiler::printMeasurements();
	/*
	try {
		Window window(
			1024,
			768,
			APPLICATION_TITLE,
			4,		// multisampling
			false,	// vertical sync
			true,	// resizeable
			true,	// decorated
			false	// center cursor
		);
		window.setClearColor(Color(0.4f, 0.4f, 0.4f, 1.0f));
		window.setKeyRepeat(false);
		Shader shader;
		shader.loadFromFile("shaders/default.vert", Shader::Type::Vertex);
		shader.loadFromFile("shaders/default.frag", Shader::Type::Fragment);

		RenderStates states;
		states.shader = &shader;
		states.transform = Transform::Identity;

		ResourceHolder<TextureID, Texture> textureHolder;
		textureHolder.load(TextureID::Eagle, "textures/eagle.png");
		textureHolder.load(TextureID::Raptor, "textures/raptor.png");
		textureHolder.load(TextureID::FMODLogo, "textures/fmod_logo.png");
		textureHolder.load(TextureID::Checkerboard, "textures/checkerboard.jpg");
		textureHolder.load(TextureID::Explosion, "textures/explosion.png");
		textureHolder.load(TextureID::Guybrush, "textures/guybrush.png");
		textureHolder.load(TextureID::ButtonNormal, "textures/button_normal.png");
		textureHolder.load(TextureID::ButtonActive, "textures/button_active.png");
		textureHolder.load(TextureID::ButtonSelected, "textures/button_selected.png");
		textureHolder.get(TextureID::Guybrush).setTextureFiltering(Texture::Filtering::Nearest);
		textureHolder.get(TextureID::Eagle).setTextureFiltering(Texture::Filtering::Nearest);
		textureHolder.get(TextureID::Raptor).setTextureFiltering(Texture::Filtering::Nearest);
		textureHolder.get(TextureID::FMODLogo).setTextureFiltering(Texture::Filtering::Nearest);
		textureHolder.get(TextureID::Checkerboard).setTextureWrap(true);

		Sprite spriteEagle;
		spriteEagle.setTexture(textureHolder.get(TextureID::Eagle));
		spriteEagle.setOrigin(-150.f, 0.f);

		Sprite spriteRaptor;
		spriteRaptor.setTexture(textureHolder.get(TextureID::Raptor));
		spriteRaptor.centerOrigin();

		Sprite fmodSprite;
		fmodSprite.setTexture(textureHolder.get(TextureID::FMODLogo));		
		fmodSprite.setScale(glm::vec2(0.3f));

		Sprite checkerboard;
		checkerboard.setTexture(textureHolder.get(TextureID::Checkerboard));
		checkerboard.setTiling(4, 4);
		checkerboard.centerOrigin();

		std::vector<Animation> animations;
		Animation guybrush;
		guybrush.setTexture(textureHolder.get(TextureID::Guybrush));
		guybrush.generateAnimationStates(6, 1, Time::seconds(0.2f));
		guybrush.centerOrigin();
		guybrush.setLooping(true);
		guybrush.setScale(-1.f, 1.f);
		guybrush.setPosition(0.f, -300.f);

		// text test
		Font font;
		font.loadFromFile("fonts/comicbd.ttf");
		Text text;
		text.setFont(font, 48);
		text.setString("Comic Sans MS");
		text.centerOrigin();
		Text text2;
		text2.setFont(font, 60);
		text2.setString("Camera Control Test");
		text2.setPosition(0.f, 200.f);
		text2.setColor(Color(0.31f, 0.4365f, 1.0f, 1.f));
		text2.centerOrigin();

		VertexArray va;
		va.append(Vertex(glm::vec2(-100.f, -100.f), Color(1.f, 0.f, 0.f)));
		va.append(Vertex(glm::vec2(100.f, -100.f), Color(0.f, 1.f, 0.f)));
		va.append(Vertex(glm::vec2(100.f, 100.f), Color(0.f, 0.f, 1.f)));
		va.append(Vertex(glm::vec2(-100.f, 100.f), Color(0.f, 1.f, 1.f)));

		int renderedFrames = 0;
		Clock clock;
		Clock fpsClock;

		// music and sound
		Music music;
		music.openFromFile("music/battleship.ogg");
		music.setVolume(0.2f);

		ResourceHolder<SoundID, SoundBuffer> soundHolder;
		soundHolder.load(SoundID::Kick, "sfx/Drums-Kick.wav");
		soundHolder.load(SoundID::Snare, "sfx/Drums-Snare.wav");
		soundHolder.load(SoundID::Hats, "sfx/Drums-Hats.wav");
		soundHolder.load(SoundID::Crash, "sfx/Drums-Crash.wav");
		soundHolder.load(SoundID::Crow, "sfx/crow.wav");

		Camera camera;

		// entity component test
		EntityContainer entityContainer;

		auto rootEntity = std::make_unique<Entity>();
		rootEntity->setPosition(300.f, 200.f);
		//rootEntity->setScale(2.f, 1.f);
		rootEntity->setRotation(glm::radians(25.f));
		Entity* rootEntityPtr = rootEntity.get();

		auto spriteEntity = std::make_unique<Entity>();
		auto spriteRenderer = std::make_unique<SpriteRenderer>();
		spriteRenderer->getSprite().setTexture(textureHolder.get(TextureID::Guybrush));
		spriteRenderer->getSprite().centerOrigin();
		spriteEntity->addComponent(std::move(spriteRenderer));
		spriteEntity->setScale(-1.f, 1.f);
		spriteEntity->setParent(*rootEntity);

		auto animationEntity = std::make_unique<Entity>();
		auto animationRenderer = std::make_unique<AnimationRenderer>();
		animationRenderer->getAnimation().setTexture(textureHolder.get(TextureID::Guybrush));
		animationRenderer->getAnimation().generateAnimationStates(6, 1, Time::seconds(0.2f));
		animationRenderer->getAnimation().centerOrigin();
		animationRenderer->getAnimation().setLooping(true);
		animationEntity->addComponent(std::move(animationRenderer));
		animationEntity->setPosition(0.f, -50.f);
		animationEntity->setParent(*rootEntity);

		auto textEntity = std::make_unique<Entity>();
		auto textRenderer = std::make_unique<TextRenderer>();
		textRenderer->getText().setFont(font, 48);
		textRenderer->getText().setString("Entity Component Model");
		textRenderer->getText().centerOrigin();
		textEntity->addComponent(std::move(textRenderer));
		textEntity->setRotation(glm::radians(-90.f));
		textEntity->centerOrigin();
		textEntity->setParent(*rootEntity);

		entityContainer.add(std::move(rootEntity));
		entityContainer.add(std::move(spriteEntity));
		entityContainer.add(std::move(animationEntity));
		entityContainer.add(std::move(textEntity));
		entityContainer.awake();

		// GUI
		GUI::Container guiContainer;
		auto button = std::make_shared<GUI::Button>();
		button->setString("Explode!");
		button->setFont(font, 24);		
		button->setNormalTexture(textureHolder.get(TextureID::ButtonNormal));
		button->setSelectedTexture(textureHolder.get(TextureID::ButtonSelected));
		button->setActiveTexture(textureHolder.get(TextureID::ButtonActive));
		button->setCallbackFunc([&window, &textureHolder, &animations]() {
			for (float x = -400.f; x <= 400.f; x += 200.f) {
				for (float y = -400.f; y <= 400.f; y += 200.f) {
					Animation explosion;
					explosion.setTexture(textureHolder.get(TextureID::Explosion));
					explosion.generateAnimationStates(4, 4, Time::milliseconds(rand() % 100 + 10));
					explosion.setPosition(x + (rand() % 100 - 50), y + (rand() % 100 - 50));
					explosion.setRotation(glm::radians(static_cast<float>(rand() % 360)));
					explosion.setScale(glm::vec2(static_cast<float>(rand() % 200) / 150.f + 0.5f));
					explosion.centerOrigin();
					animations.push_back(explosion);
				}
			}			
		});
		guiContainer.pack(button);

		while (!window.shouldClose()) {
			// update
			AudioSystem::update();

			static float lastElapsed = clock.getElapsedTime().asSeconds();
			float elapsed = clock.getElapsedTime().asSeconds();
			float dt = elapsed - lastElapsed;
			lastElapsed = elapsed;

			entityContainer.update(Time::seconds(dt));

			float rotation = glm::radians(80.f * elapsed);
			spriteRaptor.setRotation(-2.f * rotation);
			spriteRaptor.setScale(std::sin(2.5f * elapsed) * 2.5f + 3.5f, 2.f);

			spriteEagle.setRotation(rotation);

			// update animations
			for (auto& animation : animations)
				animation.update();
			guybrush.update();
			// erase animations that are not playing anymore
			animations.erase(std::remove_if(animations.begin(), animations.end(), [](const auto& animation) -> bool {
					return !animation.isPlaying();
				}),
				animations.end()
			);

			text.setScale(std::sin(2.0f * elapsed) * 1.5f + 2.f, 1.2f);
			text.setRotation(0.3f * rotation);

			// set static positions
			fmodSprite.setPosition(
				static_cast<float>(-window.getSize().x / 2 + 10),
				static_cast<float>(-window.getSize().y / 2 + 10)
			);
			button->setPosition(window.getSize().x / 2.f - 20.f - button->getWidth(), -window.getSize().y / 2.f + 20.f);
			auto mousePos = window.windowToWorldCoords(window.getMousePosition(), camera);
			rootEntityPtr->setPosition(mousePos.x, mousePos.y);

			// handle real-time input
			static constexpr float movementSpeed = 600.f;
			static constexpr float rotationSpeed = glm::radians(70.f);
			glm::vec2 movement = glm::vec2(0.f);
			if (window.isKeyPressed(Key::A))
				movement += glm::vec2(-1.f, 0.f);
			if (window.isKeyPressed(Key::D))
				movement += glm::vec2(1.f, 0.f);
			if (window.isKeyPressed(Key::W))
				movement += glm::vec2(0.f, 1.f);
			if (window.isKeyPressed(Key::S))
				movement += glm::vec2(0.f, -1.f);
			if (glm::length(movement) > 0.f)
				movement *= movementSpeed / static_cast<float>(glm::length(movement));
			camera.move(movement * dt);

			movement = glm::vec2(1.f, 0.f) * EventHandler::getGamepadAxis(Joystick::Joystick0, GamepadAxis::LeftX)
				+ glm::vec2(0.f, -1.f) * EventHandler::getGamepadAxis(Joystick::Joystick0, GamepadAxis::LeftY);
			if (glm::length(movement) > 0.2f) {
				movement = movement * static_cast<float>(glm::length(movement)) / std::sqrt(2.f) * movementSpeed;
				camera.move(movement * dt);
			}
			float zoom = -EventHandler::getGamepadAxis(Joystick::Joystick0, GamepadAxis::RightY);
			if (std::abs(zoom) > 0.2f) {
				camera.zoom(1.f + zoom * 4.f * dt);
			}				

			if (window.isKeyPressed(Key::Q) || EventHandler::isGamepadButtonPressed(Joystick::Joystick0, GamepadButton::LeftBumper))
				camera.rotate(rotationSpeed * dt);
			if (window.isKeyPressed(Key::E) || EventHandler::isGamepadButtonPressed(Joystick::Joystick0, GamepadButton::RightBumper))
				camera.rotate(-rotationSpeed * dt);

			// handle events
			Window::processEvents();
			while (window.hasEvent()) {
				Event e = window.pollEvent();
				if (guiContainer.handleEvent(e, window))
					continue;
				switch (e.type) {
					case Event::Type::JoystickConnected:
						std::cout << "Joystick/Gamepad " << static_cast<int>(e.joystick) << " connected\n";
						break;
					case Event::Type::JoystickDisconnected:
						std::cout << "Joystick/Gamepad " << static_cast<int>(e.joystick) << " disconnected\n";
						break;
					case Event::Type::GamepadButtonPress:
						switch (e.gamepadButton) {
							case GamepadButton::A:
							{
								Sound drum;
								drum.setSoundBuffer(soundHolder.get(SoundID::Kick));
								AudioSystem::playSound(drum);
								break;
							}
							case GamepadButton::X:
							{
								Sound drum;
								drum.setSoundBuffer(soundHolder.get(SoundID::Snare));
								AudioSystem::playSound(drum);
								break;
							}
							case GamepadButton::RightBumper:
							{
								Sound drum;
								drum.setSoundBuffer(soundHolder.get(SoundID::Crash));
								AudioSystem::playSound(drum);
								break;
							}
							case GamepadButton::Y:
							{
								Sound sound;
								sound.setSoundBuffer(soundHolder.get(SoundID::Crow));
								AudioSystem::playSound(sound);
								break;
							}
						}
						break;					
					case Event::Type::MouseScroll:
						camera.zoom(1.f + 0.1f * static_cast<float>(e.mouseScrollDelta.y));
						break;
					case Event::Type::MouseButtonPress:
						if (e.mouseButton == MouseButton::Left) {
							glm::vec2 position = glm::vec2(static_cast<float>(e.mousePosition.x), static_cast<float>(e.mousePosition.y));
							position = window.windowToWorldCoords(position, camera);
							Animation explosion;
							explosion.setTexture(textureHolder.get(TextureID::Explosion));
							explosion.generateAnimationStates(4, 4, Time::seconds(0.035f));
							explosion.setPosition(position.x, position.y);
							explosion.centerOrigin();
							animations.push_back(explosion);
						}
						break;
					case Event::Type::KeyPress:
						switch (e.key) {
							case Key::Escape:
								window.setWindowShouldClose(true);
								break;
							case Key::C:
							{								
								Sound sound;
								sound.setSoundBuffer(soundHolder.get(SoundID::Crow));
								AudioSystem::playSound(sound);
								break;
							}
							case Key::V:
							{
								Sound drum;
								drum.setSoundBuffer(soundHolder.get(SoundID::Kick));
								AudioSystem::playSound(drum);
								break;
							}
							case Key::F:
							{
								Sound drum;
								drum.setSoundBuffer(soundHolder.get(SoundID::Snare));
								AudioSystem::playSound(drum);
								break;
							}
							case Key::N:
							{
								Sound drum;
								drum.setSoundBuffer(soundHolder.get(SoundID::Hats));
								AudioSystem::playSound(drum);
								break;
							}
							case Key::J:
							{
								Sound drum;
								drum.setSoundBuffer(soundHolder.get(SoundID::Crash));
								AudioSystem::playSound(drum);
								break;
							}
							case Key::M:
								if (music.isPlaying())
									music.stop();
								else {
									//music.openFromFile("music/battleship.ogg");
									music.play();
								}
								break;
						}
						break;
					case Event::Type::KeyRelease:
						break;						
				}
			}

			// render
			window.clear();			

			window.draw(checkerboard, camera);
			window.draw(va, camera);
			window.draw(spriteRaptor, camera);
			window.draw(spriteEagle, camera);
			window.draw(text, camera);
			window.draw(text2, camera);

			window.draw(guybrush, camera);
			for (const auto& animation : animations)
				window.draw(animation, camera);

			window.draw(entityContainer, camera);

			// render GUI
			window.draw(fmodSprite);
			window.draw(guiContainer);
			
			window.swapBuffers();
			renderedFrames++;

			// show fps in title bar
			if (fpsClock.getElapsedTime() >= Time::seconds(1.f)) {
				float duration = fpsClock.restart().asSeconds();
				float fps = static_cast<float>(renderedFrames) / duration;
				window.setTitle(APPLICATION_TITLE + " ("s + std::to_string(fps) + " fps)"s);
				renderedFrames = 0;
			}
		}		
	} catch (const std::exception& e) {
		std::cout << "EXCEPTION: " << e.what() << std::endl;
		std::cin.get();
	}
	ScopedProfiler::printMeasurements();*/
}