#include "myApplication.h"
#include "openGLstuff.h"

#include "SOIL/SOIL.h"

void canvas(GLuint& vbo, GLuint& vao, GLuint& shaderprogram){

	//could be done with 4 points... hmm...
	float points[] = {
			-1.0f,  1.0f,  0.0f,
			-1.0f, -1.0f,  0.0f,
			 1.0f, -1.0f,  0.0f,
			-1.0f,  1.0f,  0.0f,
			 1.0f, -1.0f,  0.0f,
			 1.0f,  1.0f,  0.0f,
	};

	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), points, GL_STATIC_DRAW);
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	shaderprogram=loadShaders("bin/shaders/canvas.vert","bin/shaders/canvas.frag");

}
//plain triangle
void setup1(GLuint& vbo, GLuint & vao, GLuint & colours_vbo, GLuint & shaders){
	//if there is not a possibility to see the back of the triangle, don't render it
		glEnable(GL_CULL_FACE); // cull face
		glCullFace(GL_BACK); // cull back face
		glFrontFace(GL_CW); // GL_CCW for counter clock-wicanvas(GLuint& vbo, GLuint& vao, GLuint& shaderprogram)se

		//-------------------------------------------------------------------------
		float points[] = {
		   0.0f,  0.5f,  0.0f,
		   0.5f, -0.5f,  0.0f,
		  -0.5f, -0.5f,  0.0f,
		};

		float colours[] = {
		  1.0f, 0.0f,  0.0f,
		  0.0f, 1.0f,  0.0f,
		  0.0f, 0.0f,  1.0f
		};




		vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

		colours_vbo = 0;
		glGenBuffers(1, &colours_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colours, GL_STATIC_DRAW);

		//now comes the opengl4 things

		vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); //binding
		glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(0); //remember to enable
		glEnableVertexAttribArray(1);

		//// old versions of openGL
		//glBindAttribLocation(shader_programme, 0, "vertex_position");
		//glBindAttribLocation(shader_programme, 1, "vertex_colour");

		shaders=loadShaders("bin/shaders/test.vert","bin/shaders/test.frag");
}

void update1(GLuint shaders, GLuint vao){
	glClearColor(0,1,1,1);
	//glClearColor(0,0.5,0.5);
	glUseProgram(shaders);
	glBindVertexArray(vao);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
//

//compute shaders


void myApplication::getKeyUpdates(GLFWwindow* window){
	int state=glfwGetKey(window,GLFW_KEY_UP);
	if (state == GLFW_PRESS)
		z-=0.005;

	state=glfwGetKey(window,GLFW_KEY_DOWN);
	if (state == GLFW_PRESS)
		z+=0.005;

	state=glfwGetKey(window,GLFW_KEY_LEFT);
	if (state == GLFW_PRESS)
		x-=0.002;

	state=glfwGetKey(window,GLFW_KEY_RIGHT);
	if (state == GLFW_PRESS)
		x+=0.002;
}

void myApplication::setup(){
	//setup1(vbo, vao, colours_vbo, shaders);

	//canvas(vbo, vao, shaders);
	//raytracer_.setup(x,y,z);
	splines_.setup();


}

void myApplication::update(){
	//update1(shaders, vao);

	/*
	GLuint tex_output=raytracer_.update(x,y,z);
	{ // normal drawing pass
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glShadeModel(GL_SMOOTH);
	    glUseProgram(shaders);
	    glBindVertexArray(vao);
	    glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D, tex_output);
	    glDrawArrays(GL_TRIANGLES, 0, 18);
	}
	*/

	splines_.draw();
}


