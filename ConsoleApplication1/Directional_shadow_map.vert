#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 DirectionalLightTransform;

void main(){
	gl_Position=DirectionalLightTransform * model *vec4(pos,1.0);
}