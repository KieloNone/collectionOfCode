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
#include "splines.h"

#include <string>

//2D-example, you can do much more with the splines than just this
class splinesExample {
public:
	void loadData(const char * file_path);
	void setup();
	void draw();
private:
	Curve curve_;
	GLuint vbo_;
	GLuint vao_;
	GLuint shaderProgram_;
	GLuint texture_;
	float time_; // could be done better

};
