
#version 330


layout(location=0) in vec3 position;
out vec2 texCoordVarying;


void main(){
	texCoordVarying =position.xy*0.5+vec2(0.5,0.5);
	gl_Position = vec4(position,1);
}