#include "../openGLstuff.h"
#include "raytracing.h"

#include "SOIL/SOIL.h"

void simpleRaytracer::setup(float &x, float & y, float&z){



	//----------------------------------------

	int width=512;int height=512;
	unsigned char * data=SOIL_load_image("src/base/raytracing/testi.png",&width, &height, 0, SOIL_LOAD_RGBA);

	if (data == NULL) {
	    std::cout << "An error occurred while loading image." << std::endl;
	    exit(EXIT_FAILURE);
	}


    glGenTextures(1, &texID_);
    glBindTexture(GL_TEXTURE_2D, texID_);
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
	tex_w_ = 512, tex_h_ = 512;
	glGenTextures(1, &tex_output_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_output_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w_, tex_h_, 0, GL_RGBA, GL_FLOAT,
	 NULL); // up until here everything is normal
	//openGL4-compute shader!!, unit, texture,level, layered, layer,access,format
	glBindImageTexture(0, tex_output_, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
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
	GLuint shadeInt=createAndCompileShader("src/base/raytracing/simpleraycasting.comp", GL_COMPUTE_SHADER);

	compshader_ = glCreateProgram();
	glAttachShader(compshader_, shadeInt);
	glLinkProgram(compshader_);
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetProgramiv(compshader_, GL_LINK_STATUS, &Result);
	glGetProgramiv(compshader_, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(compshader_, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	glDetachShader(compshader_, shadeInt);
	glDeleteShader(shadeInt);

	//--------------creating shader ends here------------------------



}


GLuint simpleRaytracer::update(float x, float y, float z){

	//matrix calculations

	mat4 matrices[4] = {translate(mat4(),vec3(x,y,z)),translate(mat4(),vec3(2,0,-12)), translate(mat4(),vec3(-2,0,-12)),mat4(1.0)};
	mat4 matrices_inv[4]={inverse(matrices[0]),inverse(matrices[1]),inverse(matrices[2]),mat4(1.0)};
	int type[4] = {1,1,1,0};



	{ // launch compute shaders! Note that I am in my own paranthesis
		glUseProgram(compshader_);
		GLuint uniform_handle = glGetUniformLocation(compshader_, "model");
		glUniformMatrix4fv(uniform_handle, 4, GL_FALSE, &matrices[0][0][0]);

		uniform_handle = glGetUniformLocation(compshader_, "model_inv");
		glUniformMatrix4fv(uniform_handle, 4, GL_FALSE, &matrices_inv[0][0][0]);

		uniform_handle = glGetUniformLocation(compshader_, "type");
		glUniform1iv(uniform_handle, 4, &type[0]);

		//uniform_handle = glGetUniformLocation(comp_shader, "tex_input");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID_);

	    glDispatchCompute((GLuint)tex_w_, (GLuint)tex_h_, 1); // 1??
	}
	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	return tex_output_;


}
