#include "myApplication.h"
#include "openGLstuff.h"

#include "SOIL/SOIL.h"

void canvas(GLuint& vbo, GLuint& vao, GLuint& shaderprogram){
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
void setup2(GLuint& tex_output, int & tex_w, int & tex_h, GLuint & shaders, float &x, float & y, float&z,GLuint &texID){



	//----------------------------------------

	/*
	CImg<unsigned char> image("bin/testi.png");
	unsigned char * data=image;
	int width = image.width();
	int height = image.height();
	*/

	int width=512;int height=512;
	unsigned char * data=SOIL_load_image("bin/testi.png",&width, &height, 0, SOIL_LOAD_RGBA);

	if (data == NULL) {
	    std::cout << "An error occurred while loading image." << std::endl;
	    exit(EXIT_FAILURE);
	}


    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glActiveTexture(GL_TEXTURE0);
    //create the texture (allocating space, selecting type etc.)
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //linear interpolation when zooming out
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //mipmaps when zooming in
    //glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /*
    int test;
    //glGetTexParameter(GL_TEXTURE_2D, GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE, &test); //arch... I can do this with a sampler, but not with a imageLoad apparently...
    std::cout << test<<std::endl;
    */
	glBindTexture(GL_TEXTURE_2D,0);
	//glBindImageTexture(1, texID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

	SOIL_free_image_data(data);
	//-----------------------creating texture for compute shader-----------------


	x=-2;y=0;z=-8;
	//dimensions of the image
	tex_w = 512, tex_h = 512;
	glGenTextures(1, &tex_output);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_output);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT,
	 NULL); // up until here everything is normal
	//openGL4!!, unit, texture,level, layered, layer,access,format
	glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindTexture(GL_TEXTURE_2D,0);


	//----------------computeshader stuff---------------------------------------

	//checking the size of the compute shader
	int work_grp_cnt[3];

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

	printf("max global (total) work group size x:%i y:%i z:%i\n",
	work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);
	//work group size of work group count??

	int work_grp_size[3];

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

	printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
	work_grp_size[0], work_grp_size[1], work_grp_size[2]);

	int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	printf("max local work group invocations %i\n", work_grp_inv);

	//--------------------checking end here

	//------------creating shader--------------------------------------------------
	GLuint shadeInt=createAndCompileShader("bin/shaders/simpleraycasting.comp", GL_COMPUTE_SHADER);

	shaders = glCreateProgram();
	glAttachShader(shaders, shadeInt);
	glLinkProgram(shaders);
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetProgramiv(shaders, GL_LINK_STATUS, &Result);
	glGetProgramiv(shaders, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(shaders, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	glDetachShader(shaders, shadeInt);
	glDeleteShader(shadeInt);

	//--------------creating shader ends here------------------------



}

void update2(GLuint tex_output,int tex_w,int tex_h,GLuint shaders, GLuint comp_shader, GLuint vao,float x, float y, float z,GLuint texID){

	//matrix calculations

	mat4 matrices[3] = {translate(mat4(),vec3(x,y,z)),translate(mat4(),vec3(2,0,-12)), mat4(1.0)};
	mat4 matrices_inv[3]={inverse(matrices[0]),inverse(matrices[1]),mat4(1.0)};
	int type[3] = {1,1,0};




	{ // launch compute shaders! Note that I am in my own paranthesis
		glUseProgram(comp_shader);
		GLuint uniform_handle = glGetUniformLocation(comp_shader, "model");
		glUniformMatrix4fv(uniform_handle, 3, GL_FALSE, &matrices[0][0][0]);

		uniform_handle = glGetUniformLocation(comp_shader, "model_inv");
		glUniformMatrix4fv(uniform_handle, 3, GL_FALSE, &matrices_inv[0][0][0]);

		uniform_handle = glGetUniformLocation(comp_shader, "type");
		glUniform1iv(uniform_handle, 3, &type[0]);

		//uniform_handle = glGetUniformLocation(comp_shader, "tex_input");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);

	    glDispatchCompute((GLuint)tex_w, (GLuint)tex_h, 1); // 1??
	}
	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

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

}

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
	canvas(vbo, vao, shaders);
	setup2(tex_output, tex_w,tex_h, comp_shader,x,y,z,texID);


}

void myApplication::update(){
	//update1(shaders, vao);
	//update2(tex_output,tex_w,tex_h,shaders, comp_shader, vao,x,y,z);
	update2(tex_output,tex_w,tex_h,shaders, comp_shader, vao,x,y,z,texID);


}


