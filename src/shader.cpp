#include "shader.h"

Shader::Shader() : ID(0)
{

}

Shader::~Shader()
{
	// glDeleteProgram(this->ID);
}

Shader& Shader::Use()
{
	glUseProgram(this->ID);
	return *this;
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
	unsigned int vShader, fShader, gShader;

	// vertex shader compilation
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vertexSource, nullptr);
	glCompileShader(vShader);
	checkCompileErrors(vShader, "VERTEX");

	// fragment shader compilation
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fragmentSource, nullptr);
	glCompileShader(fShader);
	checkCompileErrors(fShader, "FRAGMENT");
	
	// geometry shader compilation if source was given
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, nullptr);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}

	// create program and attach shaders
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vShader);
	glAttachShader(this->ID, fShader);
	if (geometrySource != nullptr)
		glAttachShader(this->ID, gShader);

	// link program
	glLinkProgram(this->ID);
	checkCompileErrors(this->ID, "PROGRAM");

	// don't forget to delete shaders to free up memory
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	if (geometrySource != nullptr)
		glDeleteShader(gShader);

}

void Shader::SetFloat(const char* name, float value, bool useShader)
{
	if (useShader)
		this->Use();
	int loc = glGetUniformLocation(this->ID, name);
	if (loc != -1)
		glUniform1f(loc, value);
	else
		std::cout << "Couldn't find uniform " << name << ".\n";
}

void Shader::SetInt(const char* name, int value, bool useShader)
{
	if (useShader)
		this->Use();
	int loc = glGetUniformLocation(this->ID, name);
	if (loc != -1)
		glUniform1i(loc, value);
	else
		std::cout << "Couldn't find uniform " << name << ".\n";
}

void Shader::SetMatrix4(const char* name, glm::mat4 matrix, GLboolean transpose, bool useShader)
{
	if (useShader)
		this->Use();
	int loc = glGetUniformLocation(this->ID, name);
	if (loc != -1)
		glUniformMatrix4fv(loc, 1, transpose, glm::value_ptr(matrix));
	else
		std::cout << "Couldn't find uniform " << name << ".\n";
}

void Shader::SetVector2f(const char* name, float x, float y, bool useShader)
{
	if (useShader)
		this->Use();
	int loc = glGetUniformLocation(this->ID, name);	
	if (loc != -1)
		glUniform2f(loc, x, y);
}

void Shader::SetVector2f(const char* name, glm::vec2 value, bool useShader)
{
	if (useShader)
		this->Use();
	int loc = glGetUniformLocation(this->ID, name);
	if (loc != -1)
		glUniform2f(loc, value.x, value.y);
	else
		std::cout << "Couldn't find uniform " << name << ".\n";
}

void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
	if (useShader)
		this->Use();
	int loc = glGetUniformLocation(this->ID, name);
	if (loc != -1)
		glUniform3f(loc, x, y, z);
	else
		std::cout << "Couldn't find uniform " << name << ".\n";
}

void Shader::SetVector3f(const char* name, glm::vec3 value, bool useShader)
{
	if (useShader)
		this->Use();
	int loc = glGetUniformLocation(this->ID, name);
	if (loc != -1)
		glUniform3f(loc, value.x, value.y, value.z);
	else
		std::cout << "Couldn't find uniform " << name << ".\n";
}


void Shader::checkCompileErrors(unsigned int object, std::string type)
{
	int success = false;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cerr << "ERROR::SHADER: Compile-time error: Type: " << type << '\n'
				<< infoLog << '\n' 
				<< "---  ------------------------------------  --\n";
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cerr << "ERROR::SHADER: Link-time error: Type: " << type << '\n'
				<< infoLog << '\n'
				<< "---  ------------------------------------  --\n";
		}
	}

}
