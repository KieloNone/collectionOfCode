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
#define GL_LOG_FILE "gl.log"



bool restart_gl_log() {
	std::ofstream logstream;
	logstream.open(GL_LOG_FILE);

  if(!logstream.is_open()) {
    fprintf(stderr,
      "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
      GL_LOG_FILE);
    return false;
  }
  time_t now = time(NULL);
  std::string date= ctime(&now);
  logstream << "GL_LOG_FILE log. local time " <<date << std::endl;
  logstream.close();

  return true;
}

//copied from http://antongerdelan.net/opengl/glcontext2.html
//change to c++
bool gl_log(const char* message, ...) { //...can give more than one input
  va_list argptr;
  FILE* file = fopen(GL_LOG_FILE, "a");
  if(!file) {
    fprintf(
      stderr,
      "ERROR: could not open GL_LOG_FILE %s file for appending\n",
      GL_LOG_FILE
    );
    return false;
  }
  va_start(argptr, message);
  vfprintf(file, message, argptr);
  va_end(argptr);
  fclose(file);
  return true;
}

bool gl_log_err(const char* message, ...) {
  va_list argptr;
  FILE* file = fopen(GL_LOG_FILE, "a");
  if(!file) {
    fprintf(stderr,
      "ERROR: could not open GL_LOG_FILE %s file for appending\n",
      GL_LOG_FILE);
    return false;
  }
  va_start(argptr, message);
  vfprintf(file, message, argptr);
  va_end(argptr);
  va_start(argptr, message);
  vfprintf(stderr, message, argptr);
  va_end(argptr);
  fclose(file);
  return true;
}

void glfw_error_callback(int error, const char* description) {
  gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

void log_gl_params() {
  GLenum params[] = {
    GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
    GL_MAX_CUBE_MAP_TEXTURE_SIZE,
    GL_MAX_DRAW_BUFFERS,
    GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
    GL_MAX_TEXTURE_IMAGE_UNITS,
    GL_MAX_TEXTURE_SIZE,
    GL_MAX_VARYING_FLOATS, //depragated in openGL4
    GL_MAX_VERTEX_ATTRIBS,
    GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
    GL_MAX_VERTEX_UNIFORM_COMPONENTS,
    GL_MAX_VIEWPORT_DIMS,
    GL_STEREO,
  };
  const char* names[] = {
    "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
    "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
    "GL_MAX_DRAW_BUFFERS",
    "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
    "GL_MAX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_TEXTURE_SIZE",
    "GL_MAX_VARYING_FLOATS",
    "GL_MAX_VERTEX_ATTRIBS",
    "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
    "GL_MAX_VIEWPORT_DIMS",
    "GL_STEREO",
  };
  gl_log("GL Context Params:\n");
  // integers - only works if the order is 0-10 integer return types
  for (int i = 0; i < 10; i++) {
    int v = 0;
    glGetIntegerv(params[i], &v);
    gl_log("%s %i\n", names[i], v);
  }
  // others
  int v[2];
  v[0] = v[1] = 0;
  glGetIntegerv(params[10], v);
  gl_log("%s %i %i\n", names[10], v[0], v[1]);
  unsigned char s = 0;
  glGetBooleanv(params[11], &s);
  gl_log("%s %u\n", names[11], (unsigned int)s);
  gl_log("-----------------------------\n");
}

void _update_fps_counter(GLFWwindow* window) {
  static double previous_seconds = glfwGetTime(); //hmm...
  static int frame_count;
  double current_seconds = glfwGetTime();
  double elapsed_seconds = current_seconds - previous_seconds;
  if (elapsed_seconds > 0.25) {
    previous_seconds = current_seconds;
    double fps = (double)frame_count / elapsed_seconds;
    char tmp[128];
    sprintf(tmp, "opengl @ fps: %.2f", fps);
    glfwSetWindowTitle(window, tmp);
    frame_count = 0;
  }
  frame_count++;
}

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//   if (key == GLFW_KEY_E && action == GLFW_PRESS)
//        activate_airship();
//}

int main(){


	 assert(restart_gl_log());


	 // start GL context and O/S window using the GLFW helper library
	 gl_log("starting GLFW\n%s\n", glfwGetVersionString());
	 glfwSetErrorCallback(glfw_error_callback);
	 //glfwSetKeyCallback(window, key_callback);
	 //glfwSetWindowSizeCallback(window, glfw_window_size_callback); //mitä tehdä kun näytön koko muuttuu
	 //jälkimmäinen parametri on oma funktio

	// Initialise GLFW
	if( !glfwInit() )
	{
	    fprintf( stderr, "Failed to initialize GLFW3\n" );
	    return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing (smoothing over 4 pixels?)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

	// Open a window and create its OpenGL context
	//check: http://www.glfw.org/docs/latest/group__monitor.html
	//http://www.glfw.org/docs/latest/group__input.html
	// GLFWmonitor* mon = glfwGetPrimaryMonitor();
	//const GLFWvidmode* vmode = glfwGetVideoMode(mon);
	GLFWwindow* window;
	window = glfwCreateWindow( 1024, 768, "Window 01", NULL, NULL);
	//full screen
	//window = glfwCreateWindow(vmode->width, vmode->height, "Extended GL Init", mon, NULL);
	if( window == NULL ){
	    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
	    glfwTerminate();
	    return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental=true; //will help in loading the latest openGL version
	if (glewInit() != GLEW_OK) {
	    fprintf(stderr, "Failed to initialize GLEW\n");
	    return -1;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	log_gl_params();

	//-------------------------------------------------------------------------

	//if there is not a possibility to see the back of the triangle, don't render it
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CW); // GL_CCW for counter clock-wise

	//-------------------------------------------------------------------------
	float points[] = {
	   0.0f,  0.5f,  0.0f,
	   0.5f, -0.5f,  0.0f,
	  -0.5f, -0.5f,  0.0f
	};

	float colours[] = {
	  1.0f, 0.0f,  0.0f,
	  0.0f, 1.0f,  0.0f,
	  0.0f, 0.0f,  1.0f
	};

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colours, GL_STATIC_DRAW);

	//now comes the opengl4 things

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0); //enable the bind vbo at 0, vao will remember this!
	glEnableVertexAttribArray(1);

	//// old versions of openGL
	//glBindAttribLocation(shader_programme, 0, "vertex_position");
	//glBindAttribLocation(shader_programme, 1, "vertex_colour");

	GLuint shaders=loadShaders("bin/shaders/test.vert","bin/shaders/test.frag");
	do{

		//glViewport(0, 0, g_gl_width, g_gl_height); //jos skreenin koko muuttuu...

		_update_fps_counter(window);
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0,1,1,1);
		//glClearColor(0,0.5,0.5);

		glUseProgram(shaders);
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);

	    // Swap buffers
	    glfwSwapBuffers(window);
	    glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0 );

}
