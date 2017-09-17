#pragma once

#include <GL/glew.h>

#define GLFW_STATIC
// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include<glm/gtx/transform.hpp>
using namespace glm;


#include "../openGLstuff.h"

#include <string>
//this work is based on a code (for CPU) that I wrote for a school course

class particleSystem {
public:
	void setup();
	void update();
private:
	GLuint tex_parameters_;
	GLuint tex_cloth_;
	GLuint tex_cloth2_;
	int tex_w_;
	int tex_h_;
	int tex_t_;
	int tex_t2_;
	float time_;
	float time0_;
	int numIndices_;
	GLuint initial_shader_;
	GLuint cloth_shader_;
	GLuint temp_shader_;

	GLuint shaderprogram_;
	GLuint vao_;

};
