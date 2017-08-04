#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>

#define GLFW_STATIC
// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "openGLstuff.h"

#include "errorLogging.h"
//#include "windowWrapper.h"

class myApplication {
	public:
		void setup();
		void update();

	private:
		GLuint vbo;
		GLuint colours_vbo;
		GLuint vao;
		GLuint shaders;

};
