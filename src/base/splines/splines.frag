#version 400
out vec4 frag_colour;
uniform sampler2D tex_input;
void main() {
	vec4 color = texture(tex_input,gl_PointCoord);
	frag_colour= color;//vec4(1.0,0.0,0.0, color.x);
  }