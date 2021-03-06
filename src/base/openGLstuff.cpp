
#define GLFW_STATIC
// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "CImg.h"
using namespace cimg_library;
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "openGLstuff.h"
#include <assimp/cimport.h>
#include <typeinfo>

#include "SOIL/SOIL.h"
using namespace std;

/*
void openGLexample::setup(){


	//loading some textures
	ofImage image0;
	ofImage image1;
	image0.load("bin/data/kissa.png");
	image1.load("bin/data/koira.png");
	//

	//data fir cubemap
	vector<const GLchar*> faces;
	faces.push_back("bin/data/sky.png");
	faces.push_back("bin/data/sky.png");
	faces.push_back("bin/data/sky.png");
	faces.push_back("bin/data/sky.png");
	faces.push_back("bin/data/sky.png");
	faces.push_back("bin/data/sky.png");


	//loading a cube, using motorsaw when a knife would have been enough
	ofxAssimpModelLoader modelLoader;
	if(modelLoader.loadModel("cube.obj",false)){
		ourMesh_=modelLoader.getMesh(0);
	}

	ourMesh2_.addVertices(ourMesh_.getVertices());

	//getting the appropriate matrixes
	ofMatrix4x4 Projection= ofMatrix4x4::newPerspectiveMatrix(45.0f, 1024.0 /768.0, 0.1f, 100.0f);

	//note1: openFrameworks builds the lookAt matrix weirdly, I use the same definition as glm
	ofMatrix4x4 View = newLookAtMatrixGLMway(ofVec3f(4,3,-3),ofVec3f(0,0,0),ofVec3f(0,1,0));

	//note2: I've edited the function ofMatrix4x4 operator * ( const ofMatrix4x4 &m ) in ofMatrix4x4 to get this right
	PV_=Projection*View;
	Model_ = ofMatrix4x4::newIdentityMatrix();

	//-------------------- openGL stuff begins here --------------------------------------------------------------------------------

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	//not sure what the following does but it is necessary:
	glGenVertexArrays(1, &VertexArrayID_);
	glBindVertexArray(VertexArrayID_);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);




	//creating shaders
	programID_[0]=loadShaders("bin/data/shader.vert","bin/data/shader.frag");
	programID_[1]=loadShaders("bin/data/cubemap.vert","bin/data/cubemap.frag");

	//creating textures
	textures_[0]=configure_texture2D(image0);
	textures_[1]=configure_texture2D(image1);
	textures_[2]=configure_cubeMap(faces);

	//our shader has texture samplers "kissa and koira". The following get's their locations
	textureLocations_[0] = glGetUniformLocation(programID_[0], "kissa");
	textureLocations_[1] = glGetUniformLocation(programID_[0], "koira");
	textureLocations_[2] = glGetUniformLocation(programID_[1], "skybox");

	//creating our object, currently just creates vertex and uv buffers...
	flag_[0]=configure_object(ourMesh_, vertexBufferID_,colorBufferID_,uvBufferID_,indexBufferID_);
	numVertices_ = ourMesh_.getNumVertices();
	flag_[1]=configure_object(ourMesh2_, vertexBufferID2_,colorBufferID_,colorBufferID_,indexBufferID_);
	numVertices_ = ourMesh_.getNumVertices();
}

void openGLexample::draw(){


	GLuint mvp_handle;#include <string>
	//we need to clear the color and depth buffers to avoid artifacts
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//telling GPU which shader program to use
	glDepthMask(GL_FALSE);
	glUseProgram(programID_[1]);
	draw_object(vertexBufferID2_,vertexBufferID2_, numVertices_,flag_[1]);
    mvp_handle = glGetUniformLocation(programID_[1], "PV");
    glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, PV_.getPtr());
	draw_texture(textures_[2],textureLocations_[2],GL_TEXTURE_CUBE_MAP,0);


	glDepthMask(GL_TRUE);

    glUseProgram(programID_[0]);

    //telling GPU to draw our vertices
    draw_object(vertexBufferID_,uvBufferID_, numVertices_,flag_[0]);

    // sending matrix "mvp" to shader location "uniform mat4 MVP"
    // also adding some rotation in model space
    mvp_handle = glGetUniformLocation(programID_[0], "MVP");
    Model_.rotate(0.1f,0,1,0);
    ofMatrix4x4 mvp=PV_*Model_;
    glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, mvp.getPtr());

    //sending textures to GPU
    draw_texture(textures_[0],textureLocations_[0],GL_TEXTURE_2D,0);
    draw_texture(textures_[1],textureLocations_[1],GL_TEXTURE_2D,1);
    */
//}




//this could be done easier with SOIL, but just to know how it is done
GLuint configure_texture2D(const char* filename, myimagetype type,int width, int height){

	//getting image data from ofImage

    unsigned char * data=SOIL_load_image(filename,&width, &height, 0, SOIL_LOAD_RGBA);

    if (data == NULL) {
        std::cout << "An error occurred while loading image." << std::endl;
        exit(EXIT_FAILURE);
    }

    GLuint textureID;
    //creating a texture pointer at address textureID
    glGenTextures(1, &textureID);
    //bind the generated texture to GL_TEXTURE_2D variable
    glBindTexture(GL_TEXTURE_2D, textureID);
    //create the texture (allocating space, selecting type etc.)
    if(type==COL_RGB)
    	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (type==COL_RGBA)
    	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //setting what to do when the area to draw the texture is either bigger of smaller than the actual texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //linear interpolation when zooming out
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //mipmaps when zooming in
    glGenerateMipmap(GL_TEXTURE_2D); //generating the mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //unbind GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;

}

void update_texture2D(GLuint textureID, string filename, myimagetype type){
    glBindTexture(GL_TEXTURE_2D, textureID);
	CImg<unsigned char> image(filename.c_str());
	unsigned char * data=image;
	int width = image.width();
	int height = image.height();
    //create the texture (allocating space, selecting type etc.)
    if(type==COL_RGB)
    	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (type==COL_RGBA)
    	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint configure_cubeMap(vector<const GLchar*> & faces, myimagetype type){

	//tutorial for this: https://learnopengl.com/?_escaped_fragment_=Advanced-OpenGL/Cubemaps#!Advanced-OpenGL/Cubemaps
	//this function is pretty similar to that of configure_texture2D with the exeption that we need to load 6 images (one for each face)

	GLuint textureID;
	//generating texture pointer
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


	for(GLuint i = 0; i < faces.size(); i++){

		CImg<unsigned char> image(faces[i]);
		unsigned char * data=image;
		int width = image.width();
		int height = image.height();

		if(type==COL_RGB){
			glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
					GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	    }else if(type==COL_RGBA){
			glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
					GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	    }
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//what to do when the texture coordinates go out of bounds
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // one addition dimension
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textureID;

}

void update_cubeMap(vector<const GLchar*> & faces,GLuint & textureID, myimagetype type){

	//tutorial for this: https://learnopengl.com/?_escaped_fragment_=Advanced-OpenGL/Cubemaps#!Advanced-OpenGL/Cubemaps
	//this function is pretty similar to that of configure_texture2D with the exeption that we need to load 6 images (one for each face)

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


	for(GLuint i = 0; i < faces.size(); i++){
		CImg<unsigned char> image(faces[i]);
		unsigned char * data=image;
		int width = image.width();
		int height = image.height();

		if(type==COL_RGB){
			glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
					GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	    }else if(type==COL_RGBA){
			glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
					GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	    }
	}
}


template <class T>
GLuint configure_array(vector<T> & data){
	GLuint bufferID;
	if((typeid(T)==typeid(vec3)) || (typeid(T)==typeid(vec2)) ){
		//Generate 1 buffer, put the resulting identifier to bufferID
		glGenBuffers(1, &bufferID);
		// bind generated buffer to GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		//create content
		T* pointer;
		#ifdef TARGET_OSX
			pointer= &data[0];
		#else
			pointer= data.data();
		#endif
		glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T) , pointer, GL_STATIC_DRAW);
		return bufferID;

	}else{
		cout << "Error in cofigure_array"<< std::endl; // TODO: error handling
		return 0;
	}


}

GLuint configure_indexarray(vector<int> & data){
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	// note that we need GL_ELEMENT_ARRAY_BUFFER!
	int* pointer;
	#ifdef TARGET_OSX
		pointer= &data[0];
	#else
		pointer= data.data();
	#endif
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(unsigned int) , pointer, GL_STATIC_DRAW);
	return indexBufferID;

}



void draw_object(GLuint vertexBufferID,GLuint  uvBufferID, GLuint normalBufferID, GLuint indexBufferArray, int numIndices, int flag){
	glEnableVertexAttribArray(0); // 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
	);
	//inserting uv to the gpu
	if(flag & 4){
		glEnableVertexAttribArray(1); //layout=1
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glVertexAttribPointer(
				1,                                // attribute.
				2,                                // size 2!!!!!!!!!!!!!!!!
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
		);
	}
	if(flag & 2){
		glEnableVertexAttribArray(2); //layout=2
		glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
		glVertexAttribPointer(
				2,                                // attribute.
				3,
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
		);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferArray);
	 glDrawElements(
	     GL_TRIANGLES,      // mode
	     numIndices,    // count
	     GL_UNSIGNED_INT,   // type
	     (void*)0           // element array buffer offset
	 );
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void draw_object(GLuint vertexBufferID,GLuint  uvBufferID, int numVertices, int flag){
	glEnableVertexAttribArray(0); // 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
	);
	//inserting uv to the gpu
	if(flag & 4){
		glEnableVertexAttribArray(1); //layout=1
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glVertexAttribPointer(
				1,                                // attribute.
				2,                                // size 2!!!!!!!!!!!!!!!!
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
		);
	}
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void draw_texture(GLuint texture,GLuint textureID, GLuint type, GLuint position){
	// activating the texture number <position>
    glActiveTexture(GL_TEXTURE0 + position);
    // binding our texture to context
    glBindTexture(type,texture);
    // saying that texture number <position> should be bound to variable textureID
    glUniform1i(textureID,position);

}

//the following 2 functions are slightly modified version of the original found from http://www.opengl-tutorial.org/

GLuint createAndCompileShader(const char * file_path, GLuint type){

	//create shader
	GLuint ShaderID = glCreateShader(type);

	// Read the shader code from the file
	string ShaderCode;
	ifstream ShaderStream(file_path, std::ios::in);
	if(ShaderStream.is_open()){
		string Line = "";
		while(getline(ShaderStream, Line))
			ShaderCode += "\n" + Line;
		ShaderStream.close();
	}else{
		printf("Impossible to open %s.", file_path);
		getchar();
		return 0;
	}

	// Compile  Shader
	printf("Compiling shader : %s\n", file_path);
	char const * ShaderSourcePointer = ShaderCode.c_str();
	glShaderSource(ShaderID, 1, &ShaderSourcePointer , NULL);
	glCompileShader(ShaderID);

	// check whether or not the compilation was successful
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		if(type==GL_VERTEX_SHADER){
			vector<char> VertexShaderErrorMessage(InfoLogLength+1);
			glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}
		else{
			vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
			glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}
	}

	return ShaderID;

}

GLuint createAndCompileShaderWithIncludes(const char * file_path, GLuint type){

	//create shader
	GLuint ShaderID = glCreateShader(type);

	// Read the shader code from the file
	string ShaderCode;
	ifstream ShaderStream(file_path, std::ios::in);
	std::string temp("#includeO");
	if(ShaderStream.is_open()){
		string Line = "";
		while(getline(ShaderStream, Line)){

			//checking for includes...
			string substring=Line.substr(0,9);

			if(substring.compare(temp)==0 ){
				std::cout << substring << std::endl;
				substring=Line.substr(10);
				std::cout << substring << std::endl;
				ifstream IncludeStream(substring, std::ios::in);
					if(IncludeStream.is_open()){
						while(getline(IncludeStream, Line)){
							ShaderCode += "\n" + Line;
						}
						IncludeStream.close();
					}else{
						std::cout << "MY:Impossible to open include ." << substring << "."<<std::endl;//printf("MY:Impossible to open include %s.", substring);
					}
			}
			else
				ShaderCode += "\n" + Line;
		}

		ShaderStream.close();
	}else{
		printf("Impossible to open %s.", file_path);
		getchar();
		return 0;
	}

	// Compile  Shader
	printf("Compiling shader : %s\n", file_path);
	char const * ShaderSourcePointer = ShaderCode.c_str();
	glShaderSource(ShaderID, 1, &ShaderSourcePointer , NULL);
	glCompileShader(ShaderID);

	// check whether or not the compilation was successful
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		if(type==GL_VERTEX_SHADER){
			vector<char> VertexShaderErrorMessage(InfoLogLength+1);
			glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}
		else{
			vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
			glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}
	}

	return ShaderID;

}

GLuint loadCompShaderWithIncludes(const char * vertex_file_path){
	GLuint shadeInt=createAndCompileShaderWithIncludes(vertex_file_path, GL_COMPUTE_SHADER);

	GLuint shaderprogram = glCreateProgram();
	glAttachShader(shaderprogram, shadeInt);
	glLinkProgram(shaderprogram);
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetProgramiv(shaderprogram, GL_LINK_STATUS, &Result);
	glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(shaderprogram, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(shaderprogram, shadeInt);
	glDeleteShader(shadeInt);
	return shaderprogram;
}

GLuint loadCompShader(const char * vertex_file_path){
	GLuint shadeInt=createAndCompileShader(vertex_file_path, GL_COMPUTE_SHADER);

	GLuint shaderprogram = glCreateProgram();
	glAttachShader(shaderprogram, shadeInt);
	glLinkProgram(shaderprogram);
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetProgramiv(shaderprogram, GL_LINK_STATUS, &Result);
	glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(shaderprogram, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(shaderprogram, shadeInt);
	glDeleteShader(shadeInt);
	return shaderprogram;
}

GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create and compile shaders
	GLuint VertexShaderID = createAndCompileShader(vertex_file_path, GL_VERTEX_SHADER);
	GLuint FragmentShaderID = createAndCompileShader(fragment_file_path, GL_FRAGMENT_SHADER);

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
