#include "resource_manager.h"

//#include "stb_image.h"

#include <fstream>
#include <sstream>

// Instantiate the static variables so that the linker can see them
// this is because static member variables are separate from the class objects
std::map<std::string, Shader> ResourceManager::Shaders;
//std::map<std::string, Texture2D> ResourceManager::Textures;

Shader ResourceManager::LoadShader(const char* vertexPath, const char* fragPath, const char* geometryPath, std::string name)
{
	Shaders[name] = loadShaderFromFile(vertexPath, fragPath, geometryPath);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

//Texture2D ResourceManager::LoadTexture(const char* texPath, bool alpha, std::string name)
//{
//	Textures[name] = loadTextureFromFile(texPath, alpha);
//	return Textures[name];
//}

//Texture2D ResourceManager::GetTexture(std::string name)
//{
//	return Textures[name];
//}

void ResourceManager::Clear()
{
	// delete all shaders
	for (auto itr : Shaders)
		glDeleteProgram(itr.second.ID);
	// delete all textures
	/*for (auto itr : Textures)
		glDeleteTextures(1, &itr.second.ID);*/
}

Shader ResourceManager::loadShaderFromFile(const char* vertexPath, const char* fragPath, const char* geometryPath)
{
	std::string vertexCode;
	std::string fragCode;
	std::string geometryCode;
	try
	{
		// create file streams
		std::ifstream vertexSourceFile(vertexPath);
		std::ifstream fragSourceFile(fragPath);
		std::stringstream vertexStream, fragStream;
		// read buffer contents into string stream
		vertexStream << vertexSourceFile.rdbuf();
		fragStream << fragSourceFile.rdbuf();
		// close the file stream objects
		vertexSourceFile.close();
		fragSourceFile.close();
		// convert streams into strings
		vertexCode = vertexStream.str();
		fragCode = fragStream.str();

		// do previous but for geometry shader if present
		if (geometryPath != nullptr)
		{
			std::ifstream geometrySourceFile(geometryPath);
			std::stringstream geometryStream;

			geometryStream << geometrySourceFile.rdbuf();
			geometrySourceFile.close();
			geometryCode = geometryStream.str();
		}
		
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Could not read shader files\n";
	}
	// convert strings into a 'c type' string for openGL use later
	const char* vertexShader = vertexCode.c_str();
	const char* fragShader = fragCode.c_str();
	const char* geometryShader = nullptr;

	if (geometryPath != nullptr)
		geometryShader = geometryCode.c_str();

	Shader shader;
	shader.Compile(vertexShader, fragShader, geometryShader);
	
	/*delete vertexShader;
	delete fragShader;
	delete geometryShader;*/
	
	return shader;
	
}

//Texture2D ResourceManager::loadTextureFromFile(const char* texPath, bool alpha)
//{
//	int width, height, nrchannels;
//	Texture2D texture;
//	if (alpha)
//	{
//		texture.ImageFormat = GL_RGBA;
//		texture.InternalFormat = GL_RGBA;
//	}
//	unsigned char* data = stbi_load(texPath, &width, &height, &nrchannels, 0);
//	texture.Generate(width, height, data);
//	stbi_image_free(data);
//	return texture;
//}