#pragma once

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

bool restart_gl_log();
bool gl_log(const char* message, ...);
bool gl_log_err(const char* message, ...);
void log_gl_params();
void _update_fps_counter(GLFWwindow* window);
