#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

class Shader {
public:
	enum class Type {
		Fragment,
		Vertex,
	};
public:
				Shader();
				~Shader();
	void		loadFromFile(const std::string& filename, Type shaderType);
	void		loadFromString(const std::string& string, Type shaderType);
	void		compile() const;
	bool		isCompiled() const;
	void		use() const;
	void		setUniformMat4fv(const std::string& name, const glm::mat4& matrix) const;
	void		setUniformBool(const std::string& name, bool value) const;
private:
	std::string	readFile(const std::string& filename) const;
private:
	std::string			mVertexShader;
	std::string			mFragmentShader;
	mutable GLuint		mShaderProgram;
	mutable bool		mNeedsCompiling;
};