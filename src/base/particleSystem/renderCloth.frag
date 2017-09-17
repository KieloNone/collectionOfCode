#version 420
out vec4 outputColor;

in vec3 color;
in vec3 normal;
in vec3 pos;

vec4 diffuse(vec3 dirL)
{
    vec3 colord=max(0.0,dot(dirL,normal))*vec3(1,1,1);
    vec4 diffuseLight = vec4(color,1.0)*vec4(colord,1.0);
    return diffuseLight;
}



void main()
{
	vec3 dirL=-normalize(vec3(-4,3,3)-pos);
	//vec3 dirL=normalize(vec3(1,-1,-0.5));
	outputColor = diffuse(dirL) + vec4(0.1,0.1,0.1,1.0)*vec4(color,1.0);
	
	//outputColor=vec4(-normal.x,normal.x,0,1);
	//outputColor=vec4(color,1);

}