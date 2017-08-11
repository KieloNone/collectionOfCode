#pragma once

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

enum myimagetype {COL_R, COL_RG, COL_RGB, COL_RGBA};

template <class T>
GLuint configure_array(std::vector<T> & data);

GLuint load_texture();
GLuint configure_texture2D(std::string filename, myimagetype type);
void update_texture2D(GLuint textureID, std::string filename, myimagetype type);

GLuint createAndCompileShader(const char * file_path, GLuint type);
GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path);
void draw_texture(GLuint texture,GLuint textureID, GLuint type, GLuint position);
GLuint configure_cubeMap(std::vector<const GLchar*> & faces, myimagetype type);
void update_cubeMap(std::vector<const GLchar*> & faces,GLuint & textureID, myimagetype type);

void draw_object(GLuint vertexBufferID,GLuint uvBufferID, int numVertices,int flag);
void draw_object(GLuint vertexBufferID,GLuint  uvBufferID, GLuint normalBufferID, GLuint indexBufferArray, int numIndices, int flag);

