#version 420

out vec4 outputColor;

in vec2 texCoordVarying;

uniform sampler2D canvas;

void main()
{

	outputColor = texture(canvas, texCoordVarying);
	
	

}