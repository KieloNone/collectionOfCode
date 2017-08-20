#version 400
layout(location=0)in vec3 vp;

void main() {
	
gl_Position = vec4(vp.x/801.0-0.5,-vp.y/801.0, 0.0,1.0);
}