#include "Shader.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include "Utility.h"

Shader::Shader()
	: mNeedsCompiling(true)
	, mShaderProgram(0)
{ }

Shader::~Shader() {}

void Shader::loadFromFile(const std::string& filename, Type shaderType) {
	if (shaderType == Type::Vertex)
		mVertexShader = readFile(filename);
	else if (shaderType == Type::Fragment)
		mFragmentShader = readFile(filename);
	else
		throw std::runtime_error("Unknown shader type!");
	mNeedsCompiling = true;
}

void Shader::loadFromString(const std::string& string, Type shaderType) {
	if (shaderType == Type::Vertex)
		mVertexShader = string;
	else if (shaderType == Type::Fragment)
		mFragmentShader = string;
	else
		throw std::runtime_error("Unknown shader type!");
	mNeedsCompiling = true;
}

void Shader::compile() const {
	auto checkForCompileErrors = [](GLuint shaderIdentifier) {
		GLint success;
		GLchar infoLog[512];
		GLCALL(glGetShaderiv(shaderIdentifier, GL_COMPILE_STATUS, &success));
		if (!success) {
			GLCALL(glGetShaderInfoLog(shaderIdentifier, 512, NULL, infoLog));
			throw std::runtime_error(infoLog);
		}
	};

	auto checkForLinkErrors = [](GLuint shaderProgram) {
		GLint success;
		GLchar infoLog[512];
		GLCALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
		if (!success) {
			GLCALL(glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog));
			throw std::runtime_error(infoLog);
		}
	};

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = mVertexShader.c_str();
	GLCALL(glShaderSource(vertexShader, 1, &vertexShaderSource, NULL));
	GLCALL(glCompileShader(vertexShader));

	checkForCompileErrors(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = mFragmentShader.c_str();
	GLCALL(glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL));
	GLCALL(glCompileShader(fragmentShader));

	checkForCompileErrors(fragmentShader);

	GLCALL(mShaderProgram = glCreateProgram());
	GLCALL(glAttachShader(mShaderProgram, vertexShader));
	GLCALL(glAttachShader(mShaderProgram, fragmentShader));
	GLCALL(glLinkProgram(mShaderProgram));

	checkForLinkErrors(mShaderProgram);

	GLCALL(glDeleteShader(vertexShader));
	GLCALL(glDeleteShader(fragmentShader));

	mNeedsCompiling = false;
}

bool Shader::isCompiled() const {
	return !mNeedsCompiling;
}

void Shader::use() const {
	if (mNeedsCompiling)
		compile();
	GLCALL(glUseProgram(mShaderProgram));
}

void Shader::setUniformMat4fv(const std::string& name, const glm::mat4& matrix) const {
	if (mNeedsCompiling)
		compile();
	use();
	GLCALL(GLint uniformLocation = glGetUniformLocation(mShaderProgram, name.c_str()));
	if (uniformLocation == -1)
		std::cout << "WARNING: Uniform " << name << " not found!\n";
	GLCALL(glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
}

void Shader::setUniformBool(const std::string& name, bool value) const {
	if (mNeedsCompiling)
		compile();
	use();
	GLCALL(GLint uniformLocation = glGetUniformLocation(mShaderProgram, name.c_str()));
	if (uniformLocation == -1)
		std::cout << "WARNING: Uniform " << name << " not found!\n";
	GLCALL(glUniform1i(uniformLocation, static_cast<int>(value)));
}

std::string Shader::readFile(const std::string& filename) const {
	std::ifstream ifs(filename, std::ifstream::in);
	if (ifs.fail()) {
		throw std::runtime_error(std::string("Could not open the shader file ") + filename);
	}
	std::string result = std::string(
		(std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>())
	);
	ifs.close();
	return result;
}
