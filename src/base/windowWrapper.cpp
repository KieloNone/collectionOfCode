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


#include "windowWrapper.h"
#include "errorLogging.h"

void glfw_error_callback(int error, const char* description) {
  gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

int g_windowHeight; //extern
int g_windowWidth;  //extern

void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
	g_windowHeight=height;
	g_windowWidth=width;
  //width_=width;
  //height_=height;

  /* update any perspective matrices used here */
}

//extern int g_windowHeight;
//extern int g_windowWidth;

void windowWrapper::getKeyUpdates(){
	application_.getKeyUpdates(window_);
}

int windowWrapper::init(int width,int height){

	assert(restart_gl_log());

	g_windowHeight=height;
	g_windowWidth=width;

	// start GL context and O/S window using the GLFW helper library
	//gl_log("starting GLFW\n%s\n", glfwGetVersionString());
	//glfwSetErrorCallback(glfw_error_callback);

	//glfwSetKeyCallback(window, key_callback);

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
	window_ = glfwCreateWindow( width, height, "Window 01", NULL, NULL);
	glfwSetWindowSizeCallback(window_,&glfw_window_size_callback);

	//full screen
	//window = glfwCreateWindow(vmode->width, vmode->height, "Extended GL Init", mon, NULL);
	if( window_ == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window_); // Initialize GLEWglViewport(0, 0, g_gl_width, g_gl_height);
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
	glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);
	log_gl_params();

	application_.setup();
	return 0;
}

void windowWrapper::run(){
	do{

			//glViewport(0, 0, g_gl_width, g_gl_height); //jos skreenin koko muuttuu...
		glViewport(0, 0, g_windowWidth, g_windowHeight);
		_update_fps_counter(window_);
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    // Swap buffers	     */
		getKeyUpdates();
		update();

	    glfwSwapBuffers(window_);
	    glfwPollEvents();

		} // Check if the ESC key was pressed or the window was closed
		while( glfwGetKey(window_, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(window_) == 0 );

}
