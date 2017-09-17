//#define GLFW_DLL
//most of this code is from:
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/
//for now at least.
// Include standard headers
#include <stdio.h>
#include <stdlib.h>

//https://khronos.org/registry/OpenGL/index_gl.php
// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>

#define GLFW_STATIC
// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "openGLstuff.h"

#include <time.h>
#include <stdarg.h>

#include "windowWrapper.h"




//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//   if (key == GLFW_KEY_E && action == GLFW_PRESS)
//        activate_airship();
//}

int main(){

	windowWrapper window(840,480);
	window.run();


}
