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

#include "myApplication.h"

class windowWrapper {
	public:
		windowWrapper(){init();}
		windowWrapper(int width, int height){init(width,height);}
		~windowWrapper(){}
		void run();
		void update(){application_.update();}
		void setup(){application_.setup();}
		void getKeyUpdates();
		//void glfw_window_size_callback(GLFWwindow* window, int width, int height);
		//void setWidth(int width){width_=width;}
		//void setHeight(int height){width_=height;}
	private:
		int init(int width=514,int height=514);
		GLFWwindow* window_;
		myApplication application_;
		//int width_;
		//int height_;


};


