#version 330 core

in vec4 vColor;
in vec2 TexCoord;
in vec3 Normal;


out vec4 color;


uniform sampler2D theTexture;


void main()
{
	color = texture(theTexture, TexCoord);

}
