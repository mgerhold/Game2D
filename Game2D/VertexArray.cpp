#include "VertexArray.h"
#include <iostream>
#include "Utility.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

VertexArray::VertexArray(PrimitiveType primitiveType, DrawType drawType)
	: mNeedsUpdate(false)
	, mDrawType(drawType)
	, mPrimitiveType(primitiveType)
{
	SCOPED_PROFILER;
	// generate Vertex Array Object
	GLCALL(glGenVertexArrays(1, &mVertexArrayObject));
	bind();
	// generate Vertex Buffer Object
	GLCALL(glGenBuffers(1, &mVertexBufferObject));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject));
	// setup Vertex Buffer Layout
	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glEnableVertexAttribArray(2));
	GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(0 * sizeof(GLfloat))));
	GLCALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GLfloat))));
	GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GLfloat))));
}

VertexArray::~VertexArray() {
	SCOPED_PROFILER;
	GLCALL(glDeleteBuffers(1, &mVertexBufferObject));
	GLCALL(glDeleteVertexArrays(1, &mVertexArrayObject));
}

void VertexArray::append(const Vertex& vertex) {
	SCOPED_PROFILER;
	mVertices.push_back(vertex);
	mNeedsUpdate = true;
}

void VertexArray::modify(size_t index, const Vertex& vertex) {
	SCOPED_PROFILER;
	if (index >= mVertices.size())
		throw std::runtime_error("Index out of range.");
	mVertices[index] = vertex;
	mNeedsUpdate = true;
}

void VertexArray::resize(size_t size) {
	mVertices.resize(size);
}

void VertexArray::clear() {
	SCOPED_PROFILER;
	mVertices.clear();
	mNeedsUpdate = true;
}

const Vertex& VertexArray::operator[](size_t index) const {
	SCOPED_PROFILER;
	return mVertices[index];
}

void VertexArray::updateBuffer() const {
	SCOPED_PROFILER;
	if (mVertices.size() > 0) {
		bind();
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject));
		GLCALL(glBufferData(
			GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex),
			&mVertices[0],
			mDrawType == DrawType::Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
		));

		/*std::cout << "Updated buffer:\n";
		size_t n = mVertices.size() * 8;
		GLfloat* data = new GLfloat[n];
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, mVertices.size() * sizeof(Vertex), data);
		for (size_t i = 0; i < n; i++) {
			std::cout << data[i] << "  ";
			if ((i + 1) % 8 == 0)
				std::cout << std::endl;
		}
		delete[] data;*/
	}
	mNeedsUpdate = false;
}

void VertexArray::bind() const {
	SCOPED_PROFILER;
	GLCALL(glBindVertexArray(mVertexArrayObject));
}

void VertexArray::draw(const Window& window, RenderStates states) const {
	SCOPED_PROFILER;
	if (mVertices.size() == 0)
		return;

	if (mNeedsUpdate)
		updateBuffer();

	bind();
	if (states.shader == nullptr)
		throw std::runtime_error("Cannot render Vertex Array Object without active shader!");
	states.shader->use();
	if (states.texture != nullptr) {
		states.texture->bind();
		states.shader->setUniformBool("uUsingTexture", true);
	} else
		states.shader->setUniformBool("uUsingTexture", false);
	states.shader->setUniformMat4fv("uTransform", states.transform.getMatrix());
	GLCALL(glDrawArrays(static_cast<GLuint>(mPrimitiveType), 0, static_cast<GLsizei>(mVertices.size())));
}
