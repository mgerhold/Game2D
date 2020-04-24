#include "Utility.h"
#include <cassert>

void checkGLErrors(const char* file, int line) {
	GLenum err;
	std::string msg;
	bool firstError = true;
	while ((err = glGetError()) != GL_NO_ERROR) {
		assert(firstError); // if this is triggered, there's a high chance that the OpenGL context is lost

		switch (err) {
		case GL_INVALID_ENUM:
			msg = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			msg = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			msg = "GL_INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			msg = "GL_STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			msg = "GL_STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			msg = "GL_OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			msg = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			msg = "Unknown GL Error";
			break;
		}
		std::cout << "OpenGL Error (" << file << ", " << line << ") " << msg << std::endl;
		firstError = false;
	}
}
