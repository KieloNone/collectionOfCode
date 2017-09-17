#version 420

out vec4 outputColor;

in vec2 texCoordVarying;

uniform sampler3D canvas;

void main()
{

	outputColor = texture(canvas, vec3(texCoordVarying,11.0/11));
	
	

}