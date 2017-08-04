#include "myApplication.h"

void myApplication::setup(){

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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glEnableVertexAttribArray(0); //enable the bind vbo at 0, vao will remember this!
		glEnableVertexAttribArray(1);

		//// old versions of openGL
		//glBindAttribLocation(shader_programme, 0, "vertex_position");
		//glBindAttribLocation(shader_programme, 1, "vertex_colour");

		shaders=loadShaders("bin/shaders/test.vert","bin/shaders/test.frag");
}

void myApplication::update(){
	glClearColor(0,1,1,1);
	//glClearColor(0,0.5,0.5);
	glUseProgram(shaders);
	glBindVertexArray(vao);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, 3);

}


