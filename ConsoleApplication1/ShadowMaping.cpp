#include "ShadowMaping.h"

ShadowMaping::ShadowMaping() {
	FBO = 0;
	shadowMap = 0;
};

bool ShadowMaping::Init(GLuint width, GLuint height) {
	ShadowWidth = width;
	ShadowHeight = height;


	glGenFramebuffers(1, &FBO);
	
	glGenTextures(1,&shadowMap);
	glBindTexture(GL_TEXTURE_2D,shadowMap);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,ShadowWidth,ShadowHeight,0, GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float bcolor[] = {1.0,1.0,1.0,1.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bcolor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,shadowMap,0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {

		std::cout << "Frame Buffer Error :  " << status << std::endl;
		std::cout << ShadowHeight << "  ,,,,, "  << ShadowWidth << std::endl;
		return false;
		
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	return true;
};

void ShadowMaping::Write() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
};

void ShadowMaping::Read(GLenum TextureUnit) {
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}
GLuint ShadowMaping::getShadowWidth()
{
	return ShadowWidth;
}
GLuint ShadowMaping::getShadowHeight()
{
	return ShadowHeight;
}
ShadowMaping::~ShadowMaping() {
	if (FBO) {
		glDeleteFramebuffers(1, &FBO);
	}
	if (shadowMap) {
		glDeleteTextures(1,&shadowMap);
	}
};
