#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include "Window.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Sprite.h"
#include "Clock.h"
#include "Font.h"
#include "Text.h"
#include "Utility.h"
#include "AudioSystem.h"
#include "Music.h"
#include "SoundBuffer.h"
#include "Sound.h"
#include "ResourceHolder.h"
#include "Camera.h"

#include <memory>

using namespace std::literals::string_literals;

#define APPLICATION_TITLE "GLFW/GLAD Application"s

enum class TextureID {
	Eagle,
	Raptor,
	FMODLogo,
	Checkerboard,
};

enum class SoundID {
	Kick,
	Snare,
	Hats,
	Crash,
	Crow,
};

int main() {
	try {
		Window window(
			800,
			600,
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

		while (!window.shouldClose()) {
			// update
			AudioSystem::update();

			static float lastElapsed = clock.getElapsedTime().asSeconds();
			float elapsed = clock.getElapsedTime().asSeconds();
			float dt = elapsed - lastElapsed;
			lastElapsed = elapsed;

			float rotation = glm::radians(80.f * elapsed);
			spriteRaptor.setRotation(-2.f * rotation);
			spriteRaptor.setScale(std::sin(2.5f * elapsed) * 2.5f + 3.5f, 2.f);

			spriteEagle.setRotation(rotation);

			text.setScale(std::sin(2.0f * elapsed) * 1.5f + 2.f, 1.2f);
			text.setRotation(0.3f * rotation);

			fmodSprite.setPosition(
				static_cast<float>(-window.getSize().x / 2 + 10),
				static_cast<float>(-window.getSize().y / 2 + 10)
			);

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
				//std::cout << "\tmovement: " << glm::to_string(movement) << ", length = " << glm::length(movement) << "\n";
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
				switch (e.type) {
					case Event::Type::JoystickConnected:
						std::cout << "Joystick/Gamepad " << static_cast<int>(e.joystick) << " connected\n";
						break;
					case Event::Type::JoystickDisconnected:
						std::cout << "Joystick/Gamepad " << static_cast<int>(e.joystick) << " disconnected\n";
						break;
					/*case Event::Type::GamepadButtonPress:
						std::cout << "Gamepad " << EventHandler::getGamepadName(e.joystick) << " button " << static_cast<int>(e.gamepadButton) << " has been pressed\n";
						break;
					case Event::Type::GamepadButtonRelease:
						std::cout << "Gamepad " << EventHandler::getGamepadName(e.joystick) << " button " << static_cast<int>(e.gamepadButton) << " has been released\n";
						break;
					case Event::Type::GamepadAxisChange:
						std::cout << "Gamepad " << EventHandler::getGamepadName(e.joystick) << " axis " << static_cast<int>(e.gamepadAxisChange.axis) << " changed to " << e.gamepadAxisChange.value << "\n";
						break;*/
					case Event::Type::MouseScroll:
						camera.zoom(1.f + 0.1f * static_cast<float>(e.mouseScrollDelta.y));
						break;
					case Event::Type::KeyPress:
						switch (e.key) {
							case Key::Escape:
								window.setWindowShouldClose(true);
								break;							
							case Key::P:
								music.play();
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

			window.draw(fmodSprite);
			
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
	ScopedProfiler::printMeasurements();
}