#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "ShadowMaping.h"

class Light
{
public:
	Light();
	Light(GLfloat shadowWidth, GLfloat shadowHeight,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity);

	ShadowMaping* getShadowMap() { return shadowMap; }
	void SetAmbientItensity(GLfloat amb);
	void SetDiffuseItensity(GLfloat diff);
	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
	ShadowMaping* shadowMap;
	glm::mat4 lightProj;
};

