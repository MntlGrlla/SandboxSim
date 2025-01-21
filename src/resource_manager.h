#pragma once
#include "shader.h"
// #include "texture.h"


#include <glad/glad.h>

#include <map>
#include <string>


class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	// static std::map<std::string, Texture2D> Textures;

	static Shader LoadShader(const char* vertexPath, const char* fragPath, const char* geometryPath, std::string name);
	static Shader GetShader(std::string name);
	// static Texture2D LoadTexture(const char* texPath, bool alpha, std::string name);
	// static Texture2D GetTexture(std::string name);
	static void Clear();

private:
	static Shader loadShaderFromFile(const char* vertexPath, const char* fragPath, const char* geometryPath = nullptr);
	// static Texture2D loadTextureFromFile(const char* texPath, bool alpha);
};