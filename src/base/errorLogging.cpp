#import "errorLogging.h"

//most of this code is from http://antongerdelan.net/opengl/index.html

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
