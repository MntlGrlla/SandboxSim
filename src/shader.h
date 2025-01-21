#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

class Shader
{
public:
	// shader state
	unsigned int ID;
	// constructor
	Shader();
	// destructor
	~Shader();
	// sets current shader as active
	Shader& Use();

	// compiles and links shader program from source code
	void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
	
	// uniform setters

	void SetFloat(const char* name, float value, bool useShader = false);
	void SetInt(const char* name, int value, bool useShader = false);
	void SetMatrix4(const char* name, glm::mat4 matrix, GLboolean transpose = GL_FALSE, bool useShader = false);
	void SetVector2f(const char* name, float x, float y, bool useShader = false);
	void SetVector2f(const char* name, glm::vec2 value, bool useShader = false);
	void SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
	void SetVector3f(const char* name, glm::vec3 value, bool useShader = false);
	
	
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};