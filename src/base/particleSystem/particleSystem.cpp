#include "particleSystem.h"
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <glm/gtx/transform.hpp>
#include "../openGLstuff.h"
#include <time.h>
#include <stdarg.h>

void particleSystem::setup(){
	tex_w_ = 16, tex_h_ = 16, tex_t_=3;


	//creating textures for compute-shader
	//creating textures...

	glGenTextures(1, &tex_cloth_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, tex_cloth_);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, tex_w_, tex_h_, tex_t_, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(1, tex_cloth_, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);


	glGenTextures(1, &tex_cloth2_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, tex_cloth2_);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, tex_w_, tex_h_, tex_t_, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_3D, 0);


	//initializing compute shader:
	initial_shader_= loadCompShader("src/base/particleSystem/resetCloth.comp");
	cloth_shader_= loadCompShaderWithIncludes("src/base/particleSystem/evalCloth.comp");
	temp_shader_= loadCompShader("src/base/particleSystem/simplePass.comp");



	//launching the initial compute shader:


	{ // launch compute shaders! Note that I am in my own paranthesis
		glUseProgram(initial_shader_);
		glDispatchCompute((GLuint)tex_w_, (GLuint)tex_h_, 1); // 1??
	}
		// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		//return tex_output_;

	//creating vertices for the cloth

	//could be done with 4 points... hmm...

	std::vector<vec3> points;
	for (int j=0; j< tex_w_; j++){
		for (int k=0; k< tex_h_; k++){
			//if(j%2){
				points.push_back(vec3(j,k,0));
			//}
			//else{
			//	points.push_back(vec3(j,tex_h_-k-1,0));
			//}

		}
	}

	std::vector<int> indices;
	for (int j=0; j< tex_w_-1;j++){
		for (int k=0; k< tex_h_-1;k++){
			indices.push_back(tex_h_*j+k);
			indices.push_back(tex_h_*j+k+1);
			indices.push_back(tex_h_*(j+1)+k);

			indices.push_back(tex_h_*j+k+1);
			indices.push_back(tex_h_*(j+1)+k+1);
			indices.push_back(tex_h_*(j+1)+k);
		}

	}

	numIndices_=indices.size();
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vec3), points.data(), GL_STATIC_DRAW);

	GLuint ibo=configure_indexarray(indices);
	vao_ = 0;
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);


	shaderprogram_=loadShaders("src/base/particleSystem/renderCloth.vert","src/base/particleSystem/renderCloth.frag");
	//std::this_thread::sleep_for (std::chrono::seconds(5));
	time_=0;


}
void particleSystem::update(){

	//std::this_thread::sleep_for (std::chrono::seconds(1));
	time_ = glfwGetTime();

	{ // launch compute shaders! Note that I am in my own paranthesis
		glUseProgram(cloth_shader_);
		glBindImageTexture(0, tex_cloth2_, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);

		GLuint location = glGetUniformLocation(cloth_shader_, "previous_pass");
		draw_texture(tex_cloth_,location, GL_TEXTURE_3D, 1);

		location = glGetUniformLocation(cloth_shader_, "time");
		glUniform1f(location,time_);


		glDispatchCompute((GLuint)tex_w_, (GLuint)tex_h_, 1); // 1??
	}
		// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	{ // launch compute shaders! Note that I am in my own paranthesis
		glUseProgram(temp_shader_);
		glBindImageTexture(0, tex_cloth_, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, tex_cloth2_);

		glDispatchCompute((GLuint)tex_w_, (GLuint)tex_h_, 1); // 1??
	}
			// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);



	mat4 view = glm::lookAt(
	    glm::vec3(2,0.2,3), // Camera is at (4,3,3), in World Space
	    glm::vec3(0,0.2,0), // and looks at the origin
	    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	    );
	mat4 projection = glm::perspective(glm::radians(30.0f),840.0f/480.0f, 0.1f,100.0f);
	mat4 matrix = projection*view*glm::rotate( mat4(1.0), 0.0f*3.14f,vec3(0.0,1.0,0.0));//{translate(mat4(),vec3(x,y,z)),translate(mat4(),vec3(2,0,-12)), translate(mat4(),vec3(-2,0,-12)),mat4(1.0)};
	{ // normal drawing pass
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glShadeModel(GL_SMOOTH);
		glUseProgram(shaderprogram_);
		glBindVertexArray(vao_);
		glActiveTexture(GL_TEXTURE0);
		glPointSize(10);
		glBindTexture(GL_TEXTURE_3D, tex_cloth_);

		GLuint location2 = glGetUniformLocation(shaderprogram_, "matrix");
		glUniformMatrix4fv(location2, 1, GL_FALSE, &matrix[0][0]);

		//glDrawArrays(GL_POINTS, 0, tex_w_*tex_h_);
		glDrawElements(GL_TRIANGLES,numIndices_, GL_UNSIGNED_INT, (void*)0);
	}
	/*{ // normal drawing pass
		glDepthFunc(GL_LESS);
		glShadeModel(GL_SMOOTH);
		glUseProgram(shaderprogram_);
		glBindVertexArray(vao_);
		glActiveTexture(GL_TEXTURE0);
		glPointSize(10);
		glBindTexture(GL_TEXTURE_3D, tex_cloth_);

		GLuint location2 = glGetUniformLocation(shaderprogram_, "matrix");
		glUniformMatrix4fv(location2, 1, GL_FALSE, &matrix[0][0]);

		glDrawArrays(GL_LINE_STRIP, 0, tex_w_*tex_h_);
		//glDrawArrays(GL_POINTS, 0, tex_w_*tex_h_);
	}*/


}
