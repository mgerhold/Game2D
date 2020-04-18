#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

#include <memory>

using namespace std::literals::string_literals;

#define APPLICATION_TITLE "GLFW/GLAD Application"s

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

		Texture textureEagle;
		textureEagle.loadFromFile("textures/eagle.png");
		textureEagle.setTextureFiltering(Texture::Filtering::Nearest);

		Texture textureRaptor;
		textureRaptor.loadFromFile("textures/raptor.png");
		textureRaptor.setTextureFiltering(Texture::Filtering::Nearest);

		Sprite spriteEagle;
		spriteEagle.setTexture(textureEagle);
		spriteEagle.setOrigin(-150.f, 0.f);

		Sprite spriteRaptor;
		spriteRaptor.setTexture(textureRaptor);
		spriteRaptor.centerOrigin();

		Texture fmodLogo;
		fmodLogo.loadFromFile("textures/fmod_logo.png");
		fmodLogo.setTextureFiltering(Texture::Filtering::Nearest);
		Sprite fmodSprite;
		fmodSprite.setTexture(fmodLogo);
		fmodSprite.setPosition(
			static_cast<float>(-window.getSize().x / 2 + 10),
			static_cast<float>(-window.getSize().y / 2 + 10)
		);
		fmodSprite.setScale(glm::vec2(0.3f));

		// text test
		Font font;
		font.loadFromFile("fonts/comicbd.ttf");
		//font.generateCharacterAtlas(48);
		Text text;
		text.setFont(font, 48);
		text.setString("Comic Sans MS");
		text.centerOrigin();
		Text text2;
		text2.setFont(font, 60);
		text2.setString("playing battleship.ogg");
		text2.setColor(Color(0.31f, 0.4365f, 1.0f, 1.f));
		//text2.setScale(glm::vec2(2.f));
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
		music.setVolume(0.4f);
		//music.play();
		//std::cout << "Playing music file...\n";
		SoundBuffer sb;
		sb.loadFromFile("sfx/crow.wav");
		Sound crowSound;
		crowSound.setSoundBuffer(sb);
		std::list<Sound> sounds;
		SoundBuffer kick, snare, hats, crash;
		kick.loadFromFile("sfx/Drums-Kick.wav");
		snare.loadFromFile("sfx/Drums-Snare.wav");
		hats.loadFromFile("sfx/Drums-Hats.wav");
		crash.loadFromFile("sfx/Drums-Crash.wav");

		while (!window.shouldClose()) {
			AudioSystem::update();
			// clean up sounds
			static size_t soundCount = sounds.size();
			sounds.remove_if([](const Sound& sound) -> bool {
				return !sound.isPlaying();
			});
			if (sounds.size() != soundCount) {
				std::cout << "Sound count has changed. Now there are " << sounds.size() << " sounds.\n";
				soundCount = sounds.size();
			}
			

			Window::processEvents();
			while (window.hasEvent()) {
				Event e = window.pollEvent();
				switch (e.type) {
					case Event::Type::KeyPress:
						std::cout << "Key was pressed\n";
						switch (e.key) {
							case Key::Escape:
								window.setWindowShouldClose(true);
								break;
							case Key::S:
								music.stop();
								break;
							case Key::P:
								music.play();
								break;
							case Key::C:
							{								
								Sound sound;
								sound.setSoundBuffer(sb);
								sounds.push_back(sound);
								sounds.back().play();
								break;
							}
							case Key::V:
							{
								Sound drum;
								drum.setSoundBuffer(kick);
								sounds.push_back(drum);
								sounds.back().play();
								break;
							}
							case Key::F:
							{
								Sound drum;
								drum.setSoundBuffer(snare);
								sounds.push_back(drum);
								sounds.back().play();
								break;
							}
							case Key::N:
							{
								Sound drum;
								drum.setSoundBuffer(hats);
								sounds.push_back(drum);
								sounds.back().play();
								break;
							}
							case Key::J:
							{
								Sound drum;
								drum.setSoundBuffer(crash);
								sounds.push_back(drum);
								sounds.back().play();
								break;
							}
							case Key::M:
								music.openFromFile("music/battleship.ogg");
								music.play();
								break;
						}
						break;
					case Event::Type::KeyRelease:
						std::cout << "Key was released\n";
						break;						
				}
			}
			window.clear();

			float elapsed = clock.getElapsedTime().asSeconds();			


			float rotation = glm::radians(80.f * elapsed);
			spriteRaptor.setRotation(-2.f*rotation);
			spriteRaptor.setScale(std::sin(2.5f*elapsed) * 2.5f + 3.5f, 2.f);
			
			spriteEagle.setRotation(rotation);

			text.setScale(std::sin(2.0f * elapsed) * 1.5f + 2.f, 1.2f);			
			text.setRotation(0.3f * rotation);

			window.draw(fmodSprite, states);
			window.draw(va, states);

			window.draw(spriteRaptor, states);
			window.draw(spriteEagle, states);			
			window.draw(text, states);
			window.draw(text2, states);
			
			window.swapBuffers();
			renderedFrames++;

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