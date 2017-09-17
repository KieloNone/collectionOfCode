#include "splinesExample.h"
#include <fstream>
#include <sstream>
#include "../openGLstuff.h"
#include <string>

void splinesExample::loadData(const char * filename){
	std::ifstream infile(filename,std::ios::in);
	float a,b;
	char c;
	std::vector<vec3> initialPoints;

	if(infile.is_open()){

		while((infile >> a >>c >> b) && (c==',')){
			initialPoints.push_back(vec3(a,b,0));

		}
		infile.close();
		}else{
			printf("Impossible to open %s.", filename);
			getchar();
		}

	curve_=evalBezier(initialPoints,21, 0, 0, 0);

}

void splinesExample::setup(){

	time_=0;
	loadData("src/base/splines/helloworld.txt");
	std::vector<vec3> points;
	for (unsigned j=0; j< curve_.size(); j++){
		points.push_back(curve_[j].V);
		//std::cout << points[j][0] << " " <<points[j][1] << " " << points[j][2] << std::endl;
	}

	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	//create content
	vec3* pointer;
	#ifdef TARGET_OSX
		pointer= &data[0];
	#else
		pointer= points.data();
	#endif
	glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(vec3) , pointer, GL_STATIC_DRAW);


	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); //binding
	glEnableVertexAttribArray(0); //remember to enable
	glPointSize(10);
	//sit shaderi
	texture_= configure_texture2D("src/base/splines/temp.png", COL_RGBA,480,480);
	shaderProgram_=loadShaders("src/base/splines/splines.vert","src/base/splines/splines.frag");

}

void splinesExample::draw(){

	time_=time_+0.01;
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0,1,1,1);
	//glClearColor(0,0.5,0.5);
	glUseProgram(shaderProgram_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glBindVertexArray(vao_);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	int points = curve_.size();
	int x= floor(time_);
	if(abs(x) < points)
		points=x;

	glDrawArrays(GL_POINTS, 0, points);
}
