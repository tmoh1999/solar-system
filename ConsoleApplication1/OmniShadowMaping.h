#pragma once
#include "ShadowMaping.h"

class OmniShadowMaping :
    public ShadowMaping
{
public:
	OmniShadowMaping();

	bool Init(unsigned int width, unsigned int height);

	void Write();

	void Read(GLenum TextureUnit);

	~OmniShadowMaping();
};

