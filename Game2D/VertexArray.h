#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "NonCopyable.h"
#include "Vertex.h"
#include "Drawable.h"

enum class PrimitiveType {
	Points = GL_POINTS,
	LineStrip = GL_LINE_STRIP,
	LineLoop = GL_LINE_LOOP,
	Lines = GL_LINES,
	TriangleStrip = GL_TRIANGLE_STRIP,
	TriangleFan = GL_TRIANGLE_FAN,
	Triangles = GL_TRIANGLES,
	QuadStrip = GL_QUAD_STRIP,
	Quads = GL_QUADS,
	Polygon = GL_POLYGON,
};

enum class DrawType {
	Static,
	Dynamic,
};

class VertexArray
	: private NonCopyable
	, public Drawable
{
public:
						VertexArray(PrimitiveType primitiveType = PrimitiveType::Quads, DrawType drawType = DrawType::Dynamic);
						~VertexArray();
	void				append(const Vertex& vertex);
	void				modify(size_t index, const Vertex& vertex);
	void				resize(size_t size);
	void				clear();
	const Vertex&		operator[](size_t index) const;
private:
	void				updateBuffer() const;
	void				bind() const;
	virtual void		draw(const Window& window, RenderStates states) const override;
private:
	mutable bool		mNeedsUpdate;
	DrawType			mDrawType;
	PrimitiveType		mPrimitiveType;
	GLuint				mVertexBufferObject;
	GLuint				mVertexArrayObject;
	std::vector<Vertex>	mVertices;
};