#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>

#define GLFW_STATIC
// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include<glm/gtx/transform.hpp>
using namespace glm;


#include "openGLstuff.h"

#include "errorLogging.h"
#include "raytracing/raytracing.h"
#include "splines/splinesExample.h"
#include "particleSystem/particleSystem.h"
//#include "windowWrapper.h"

class myApplication {
	public:
		void setup();
		void update();
		void getKeyUpdates(GLFWwindow* window);

	private:
		GLuint vbo;
		GLuint colours_vbo;
		GLuint vao;
		GLuint shaders;
		GLuint tex_output;
		GLuint comp_shader;
		GLuint texID;
		simpleRaytracer raytracer_;
		splinesExample splines_;
		particleSystem cloth_;

		int tex_w; int tex_h;
		float x,y,z;
		float timer_;

};
