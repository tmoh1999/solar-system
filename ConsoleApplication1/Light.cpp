#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	shadowMap = new ShadowMaping();
	shadowMap->Init(shadowWidth, shadowHeight);

	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}
void Light::SetAmbientItensity(GLfloat amb)
{
	ambientIntensity = amb;
}
void Light::SetDiffuseItensity(GLfloat diff)
{
	diffuseIntensity = diff;
}
Light::~Light()
{
}
