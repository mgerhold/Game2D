#pragma once

#include "Image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "NonCopyable.h"


class Texture : private NonCopyable {
public:
	enum class Filtering {
		Linear,
		Nearest,
	};

							Texture();
							Texture(Texture&& other) noexcept;
							~Texture();
	bool					loadFromFile(const std::string& filename);
	bool					loadFromImage(const Image& image);
	bool					loadFromMemory(int width, int height, const unsigned char* data);
	void					bind() const;
	void					setTextureFiltering(Filtering filtering) const;
	void					setTextureWrap(bool wrap);
	glm::ivec2				getSize() const;

	static void				unbind();

private:
	GLuint					mTexture;
	bool					mLoaded;
	int						mWidth;
	int						mHeight;
};