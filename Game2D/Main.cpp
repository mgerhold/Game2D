#include <iostream>
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

#include <memory>

#define APPLICATION_TITLE "GLFW/GLAD Application"

int main() {
	try {
		Window window(
			640,
			480,
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

		// text test
		Font font;
		font.loadFromFile("fonts/comicbd.ttf");
		//font.generateCharacterAtlas(48);
		Text text;
		text.setFont(font, 48);
		text.setString("Hello World pq.-.!");
		text.centerOrigin();
		Text text2;
		text2.setFont(font, 60);
		text2.setString("Test String #2");
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

		while (!window.shouldClose()) {			
			Window::processEvents();
			while (window.hasEvent()) {
				Event e = window.pollEvent();
				switch (e.type) {
					case Event::Type::KeyPress:
						std::cout << "Key was pressed\n";
						spriteRaptor.setOrigin(0.f, 0.f);
						break;
					case Event::Type::KeyRelease:
						std::cout << "Key was released\n";
						spriteRaptor.centerOrigin();
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
				const std::string title = std::string(APPLICATION_TITLE) + " (" + std::to_string(fps) + " fps)";
				window.setTitle(title);
				//std::cout << "Rendered " << renderedFrames << " in " << duration << " seconds => " << fps << " fps\n";
				renderedFrames = 0;
			}
		}		
	} catch (const std::exception& e) {
		std::cout << "EXCEPTION: " << e.what() << std::endl;
		std::cin.get();
	}
	ScopedProfiler::printMeasurements();
}