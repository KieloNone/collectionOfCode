#pragma once

//This file is part of my schoolwork
//it has been modified to be able to use glm, GLEW etc.
//there would also be splines in glm, so this class is not that necessary

#include <GL/glew.h>

#define GLFW_STATIC
// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include<glm/gtx/transform.hpp>
using namespace glm;


#include "../openGLstuff.h"


#include <vector>

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V), the tangent (T), the
// normal (N), and the binormal (B).  It is the responsiblility of
// functions that create these objects to fill in all the data.
struct CurvePoint
{
    vec3 V; // Vertex
    vec3 T; // Tangent  (unit)
    vec3 N; // Normal   (unit)
    vec3 B; // Binormal (unit)
	vec3 K; // curvature (for curvature extra...)
	float	  t; // time	 (necessary only for the adaptive tessellation extra)
};

// This is just a handy shortcut.
typedef std::vector<CurvePoint> Curve;



////////////////////////////////////////////////////////////////////////////
// The following two functions take an array of control points (stored
// in P) and generate an STL Vector of CurvePoints.  They
// return an empty array if the number of control points in C is
// inconsistent with the type of curve.  In both these functions,
// "step" indicates the number of samples PER PIECE.  E.g., a
// 7-control-point Bezier curve will have two pieces (and the 4th
// control point is shared).
////////////////////////////////////////////////////////////////////////////

// Assume number of control points properly specifies a piecewise
// Bezier curve.  I.e., C.size() == 4 + 3*n, n=0,1,...
// points in total (P.size-1)/3*steps ?
Curve evalBezier(const std::vector<vec3>& P, unsigned steps, bool adaptive, float errorbound, float minstep);

// Bsplines only require that there are at least 4 control points.
Curve evalBspline(const std::vector<vec3>& P, unsigned steps, bool adaptive, float errorbound, float minstep);

//Catmull-Rom Splines need at least 4 but preferably more control points
Curve evalCatmullRom(const std::vector<vec3>& P, unsigned steps, bool adaptive, float errorbound, float minstep);

//create basic trefoilKnot
Curve evalTrefoilKnot(unsigned steps,float size);

// Create a circle on the xy-plane of radius and steps
Curve evalCircle(float radius, unsigned steps);



