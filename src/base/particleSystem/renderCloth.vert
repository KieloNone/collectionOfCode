#version 420


layout(location=0) in vec3 position;

uniform sampler3D positions;
uniform mat4 matrix;

out vec3 color; 
out vec3 normal;
out vec3 pos;

void main(){
	vec3 texCoord =vec3(position.xy/(16.0-1),0);
	//vec3 texCoord2 =vec3(position.xy/(16.0-1),0.5);
	vec3 texCoord2 =vec3(position.xy/(16.0-1),1);
	vec4 newPos= texture(positions,texCoord);
	pos=newPos.xyz;
	color=vec3(texCoord.xy,1); 
	normal=texture(positions,texCoord2).xyz;
	gl_Position = matrix*vec4(newPos.xyz,1);
}