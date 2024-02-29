#pragma once
#include<GL/glew.h>
#include<iostream>
class ShadowMaping
{
public:
	ShadowMaping();
	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);
	GLuint getShadowWidth();
	GLuint getShadowHeight();
	~ShadowMaping();

protected:

	GLuint FBO, shadowMap;
	GLuint ShadowWidth, ShadowHeight;

};

