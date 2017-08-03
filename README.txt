to build this project in Ubuntu I needed:
1. install libglew from repos
2. install libglm from repos
3. install dependencies for libglfw3 from repos 
4. install libglfw3 (I needed to install it by hand
(https://stackoverflow.com/questions/17768008/how-to-build-install-glfw-3-and-use-it-in-a-linux-project)
to obtain the libglfw3.a-file)
4. -l flags for the compiler: 
GL - some opengl stuff
GLU - on top of opengl (spheres, collisions)
glfw3 - opengl window, events etc
X11 - X, the graphics manager...
Xxf86vm - video mode timing
Xrandr - "rezise and rotate" windows on X
pthread - posix threads, multithread
Xi - X11 extension library(development header)
dl -dynamic library loader
GLEW - openGL version check
Xinerama - multidisplay thing
Xcursor - locate and load cursor
