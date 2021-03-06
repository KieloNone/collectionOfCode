#pragma once
// Include GLEW.
#include <GL/glew.h>

#define GLFW_STATIC
// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include<glm/gtx/transform.hpp>
using namespace glm;


#include "../openGLstuff.h"



class simpleRaytracer {
	public:

		void setup(float &x, float & y, float&z);
		GLuint update(float x, float y, float z);
		void getKeyUpdates(GLFWwindow* window);

	private:
		GLuint tex_output_;
		GLuint compshader_;
		GLuint texID_;
		int tex_w_; int tex_h_;


};

