#pragma once

#include <GL\glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
	bool LoadTextureA();

	void UseTexture();
	void ClearTexture();
	void SetTextureUnit(unsigned int tx);
	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;
	unsigned int textureUnit;
	const char* fileLocation;
};

