#include "splines.h"
//#include "extra.h"
//#ifdef WIN32
//#include <windows.h>
//#endif
//#include <GL/gl.h>
using namespace std;
//using namespace FW;

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <assert.h>
// Approximately equal to.  We don't want to use == because of
// precision issues with floating point.
inline bool approx(const vec3 lhs, const vec3 rhs) {
	const float eps = 1e-8f;
	return (lhs - rhs).length() < eps;

}

inline bool approx(float a, float b) {
	const float eps = 1e-8f;
	return abs(abs(a)-abs(b)) < eps;

}

//temporary
void setRow(mat4& matrix, int index, vec4 data){
	assert(index >= 0 && index < 4);
	for(int i = 0; i < 4; ++i)
		matrix[i][index] = data[i];

}

void setColumn(mat4& matrix, int index, vec4 data){
	assert(index >= 0 && index < 4);
	for(int i = 0; i < 4; ++i)
		matrix[index][i] = data[i];

}


// This is the core routine of the curve evaluation code. Unlike
// evalBezier, this is only designed to work on 4 control points.
// Furthermore, it requires you to specify an initial binormal
// Binit, which is iteratively propagated throughout the curve as
// the curvepoints are generated. Any other function that creates
// cubic splines can use this function by a corresponding change
// of basis.
Curve coreBezier(const vec3& p0,
				 const vec3& p1,
				 const vec3& p2,
				 const vec3& p3,
				 const vec3& Binit,
				 unsigned steps) {


	Curve R(steps+1);
	float t = 0.0f, tstep = 1.0f/steps;
	vec4 time, Bt;


	mat4 B, dB, pmatrix;

	//matrix

	setRow(B,0, vec4(1.0, -3.0,  3.0, -1.0));
	setRow(B,1, vec4(0.0,  3.0, -6.0,  3.0));
	setRow(B,2, vec4(0.0,  0.0,  3.0, -3.0));
	setRow(B,3, vec4(0.0,  0.0,  0.0,  1.0));


	//derivative for tangent (the last column will be unused!)
	setRow(dB,0, vec4(-3.0,  6.0, -3.0, 0.0));
	setRow(dB,1, vec4( 3.0,-12.0,  9.0, 0.0));
	setRow(dB,2, vec4( 0.0,  6.0, -9.0, 0.0));
	setRow(dB,3, vec4( 0.0,  0.0,  3.0, 0.0));




	for (unsigned i = 0; i <= steps; ++i) {
		time = vec4(1.0, t, t*t, t*t*t);

		//calculating data point
		Bt = B*time;
		R[i].V.x = p0.x*Bt[0] + p1.x*Bt[1] + p2.x*Bt[2] + p3.x*Bt[3];
		R[i].V.y = p0.y*Bt[0] + p1.y*Bt[1] + p2.y*Bt[2] + p3.y*Bt[3];
		R[i].V.z = p0.z*Bt[0] + p1.z*Bt[1] + p2.z*Bt[2] + p3.z*Bt[3];

		//calculating tangent (note 1/4 of all operations will be meaningless (multiplication by zero...)
		Bt = dB*time;
		R[i].T.x = p0.x*Bt[0] + p1.x*Bt[1] + p2.x*Bt[2] + p3.x*Bt[3];
		R[i].T.y = p0.y*Bt[0] + p1.y*Bt[1] + p2.y*Bt[2] + p3.y*Bt[3];
		R[i].T.z = p0.z*Bt[0] + p1.z*Bt[1] + p2.z*Bt[2] + p3.z*Bt[3];
		R[i].T=normalize(R[i].T);


		//curvature
		Bt = vec4(6.0f - 6.0f*t, -12.0f + 18.0f*t, 6.0f - 18.0f*t, 6.0f*t);
		R[i].K.x = p0.x*Bt[0] + p1.x*Bt[1] + p2.x*Bt[2] + p3.x*Bt[3];
		R[i].K.y = p0.y*Bt[0] + p1.y*Bt[1] + p2.y*Bt[2] + p3.y*Bt[3];
		R[i].K.z = p0.z*Bt[0] + p1.z*Bt[1] + p2.z*Bt[2] + p3.z*Bt[3];
		//recursive sheme for calculating binormals and tangents (see handout2 appendix)
		// as noted below we assume G1 continuity
		if (i == 0)
		{
			R[i].N = normalize(cross(Binit, R[i].T));
			//check if Binit happened to be pointing to same direction as T (this might be better with < than approx...)
			if (approx(R[i].N.length(), 0.0))
				R[i].N = normalize(cross(vec3(Binit[1], Binit[2], Binit[0]), R[i].T));

		}
		else
			R[i].N = normalize(cross(R[i-1].B, R[i].T));

		R[i].B = normalize(cross(R[i].T, R[i].N));


		t = t + tstep;

	}
	return R;
}


void quicFixNormals(Curve &data)
{
	vec3 binormal = (data[0].B + data[data.size() - 1].B)*0.5f;
	data[0].N = cross(binormal, data[0].T);
	data[0].B = cross(data[0].T, data[0].N);
	data[data.size() - 1].N = cross(binormal, data[data.size()-1].T);
	data[data.size()-1].B = cross(data[data.size()-1].T, data[data.size()-1].N);


	if (data.size() > 8)
	{
		for (unsigned i = 1; i < 4; i++)
		{
			binormal = (data[i - 1].B + data[i].B)*0.5f;
			data[i].N = cross(binormal, data[i].T);
			data[i].B = cross(data[i].T, data[i].N);

			binormal = (data[data.size()-1-i+1].B + data[data.size()-1-i].B)*0.5f;
			data[data.size()-1-i].N = cross(binormal, data[data.size()-1-i].T);
			data[data.size()-1-i].B = cross(data[data.size()-1-i].T, data[data.size()-1-i].N);

		}
	}

}




Curve coreBezier(const vec3& p0,
	const vec3& p1,
	const vec3& p2,
	const vec3& p3,
	const vec3& Binit, int level, const float errorbound, const float minstep) {


	unsigned steps = (int)(1.0 / minstep);
	float tstep = 1.0f / steps;
	Curve R(steps + 1);
	float t = 0.0f;
	vec4 time, Bt;


	mat4 B, dB, pmatrix;
	setRow(B,0, vec4(1.0, -3.0, 3.0, -1.0));
	setRow(B,1, vec4(0.0, 3.0, -6.0, 3.0));
	setRow(B,2, vec4(0.0, 0.0, 3.0, -3.0));
	setRow(B,3, vec4(0.0, 0.0, 0.0, 1.0));


	//derivative for tangent (the last column will be unused!)
	setRow(dB,0, vec4(-3.0, 6.0, -3.0, 0.0));
	setRow(dB,1, vec4(3.0, -12.0, 9.0, 0.0));
	setRow(dB,2, vec4(0.0, 6.0, -9.0, 0.0));
	setRow(dB,3, vec4(0.0, 0.0, 3.0, 0.0));

	vec3 error;


	for (unsigned i = 0; i <= steps; ++i) {
		time = vec4(1.0, t, t*t, t*t*t);

		//calculating tangent (note 1/4 of all operations will be meaningless (multiplication by zero...)
		Bt = dB*time;
		R[i].T.x = p0.x*Bt[0] + p1.x*Bt[1] + p2.x*Bt[2] + p3.x*Bt[3];
		R[i].T.y = p0.y*Bt[0] + p1.y*Bt[1] + p2.y*Bt[2] + p3.y*Bt[3];
		R[i].T.z = p0.z*Bt[0] + p1.z*Bt[1] + p2.z*Bt[2] + p3.z*Bt[3];
		R[i].T = normalize(R[i].T);

		//recursive sheme for calculating binormals and tangents (see handout2 appendix)
		// as noted below we assume G1 continuity

		if (i>0)
		{
			error = abs(R[i].T - R[i - 1].T);
		}


		t = t + tstep;

	}

	float toterror = dot(error, error);


	if (toterror < errorbound || level > 4)
	{
		t = 0;
		for (unsigned i = 0; i <= steps; ++i) {
			time = vec4(1.0, t, t*t, t*t*t);

			//calculating data point
			Bt = B*time;
			R[i].V.x = p0.x*Bt[0] + p1.x*Bt[1] + p2.x*Bt[2] + p3.x*Bt[3];
			R[i].V.y = p0.y*Bt[0] + p1.y*Bt[1] + p2.y*Bt[2] + p3.y*Bt[3];
			R[i].V.z = p0.z*Bt[0] + p1.z*Bt[1] + p2.z*Bt[2] + p3.z*Bt[3];

			//recursive sheme for calculating binormals and tangents (see handout2 appendix)
			// as noted below we assume G1 continuity
			if (i == 0)
			{
				R[i].N = normalize(cross(Binit, R[i].T));
				//check if Binit happened to be pointing to same direction as T (this might be better with < than approx...
				if (approx(R[i].N.length(), 0.0))
					R[i].N = normalize(cross(vec3(Binit[1], Binit[2], Binit[0]), R[i].T));

			}
			else
			{
				R[i].N = normalize(cross(R[i - 1].B, R[i].T));
			}

			R[i].B = normalize(cross(R[i].T, R[i].N));

			t = t + tstep;

		}
		return R;
	}
	else
	{
		vec3 p00, p01, p02, p10, p12, pmid, time;

		//midpoints of old curves
		p00 = (p1 + p0)*0.5f; //p0+(p1-p0)/2=p0-0.5p0+0.5p1=
		p01 = (p2 + p1)*0.5f;
		p02 = (p3 + p2)*0.5f;

		//midpoint of midpoints
		p10 = (p01 + p00)*0.5f;
		p12 = (p02 + p01)*0.5f;

		//midpoint of everything
		pmid = (p12 + p10)*0.5f;

		Curve templeft = coreBezier(p0, p00, p10, pmid, Binit, level + 1, errorbound, minstep);
		Curve tempright = coreBezier(pmid, p12, p02, p3, templeft[templeft.size() - 2].B, level+1,errorbound, minstep);
		Curve temp;
		for (unsigned j = 0u; j < templeft.size(); j++)
			temp.push_back(templeft[j]);
		for (unsigned j = 0u; j < tempright.size(); j++)
			temp.push_back(tempright[j]);
		return temp;

	}

}

// the P argument holds the control points and steps gives the amount of uniform tessellation.
// the rest of the arguments are for the adaptive tessellation extra.
Curve evalBezier(const vector<vec3>& P, unsigned steps, bool adaptive, float errorbound, float minstep) {
    // Check
    if (P.size() < 4 || P.size() % 3 != 1) {
        cerr << "evalBezier must be called with 3n+1 control points." << endl;
		//_CrtDbgBreak();
		exit(0);
	}

	size_t N = (P.size()-1)/3;
	cerr << "N "<< N << endl;
	Curve BezC, temp;
	vec3 Binit = vec3(0.0, 0.0, 1.0);
	for (size_t i = 0; i < N; i++)
	{
		if (adaptive)
			temp = coreBezier(P[i * 3], P[i * 3 + 1], P[i * 3 + 2], P[i * 3 + 3], Binit, 0, errorbound, minstep);
		else
			temp = coreBezier(P[i * 3], P[i * 3 + 1], P[i * 3 + 2], P[i * 3 + 3], Binit, steps);
		Binit = temp[steps].B;

		for (unsigned j = 0u; j < steps; j++)
			BezC.push_back(temp[j]);

		//cerr << "\t>>> "; printTranspose(P[i * 3]); cerr << endl;
		//cerr << "\t>>> "; printTranspose(temp[0].V); cerr << endl;
		//cerr << "\t>>> "; printTranspose(temp[steps].V); cerr << endl;

	}
	BezC.push_back(temp[steps]);
	if (approx(BezC[0].V, BezC[BezC.size() - 1].V) && !approx(BezC[0].B, BezC[BezC.size() - 1].B))
		quicFixNormals(BezC);

	/*
    cerr << "\t>>> evalBezier has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector<Vec3f>): "<< endl;
    /*for (unsigned i = 0; i < P.size(); ++i) {
        cerr << "\t>>> "; printTranspose(P[i]); cerr << endl;
    }*/

	/*
    cerr << "\t>>> Steps (type steps): " << steps << endl;
    cerr << "\t>>> Returning curve with following points." << endl;
	cout << BezC.size() << endl;
	*/
    return BezC;
}

// the P argument holds the control points and steps gives the amount of uniform tessellation.
// the rest of the arguments are for the adaptive tessellation extra.
Curve evalBspline(const vector<vec3>& P, unsigned steps, bool adaptive, float errorbound, float minstep) {
    // Check
    if (P.size() < 4) {
        cerr << "evalBspline must be called with 4 or more control points." << endl;
        exit(0);
    }

    // YOUR CODE HERE (R2):
    // EM: implementation of R2 via change of basis

	mat4 Bbezier, Bspline, BB, G1, G2;
	vec3 p0, p1, p2, p3,Binit;

	setRow(Bbezier,0, vec4(1.0, -3.0, 3.0, -1.0));
	setRow(Bbezier,1, vec4(0.0, 3.0, -6.0, 3.0));
	setRow(Bbezier,2, vec4(0.0, 0.0, 3.0, -3.0));
	setRow(Bbezier,3, vec4(0.0, 0.0, 0.0, 1.0));

	setRow(Bspline,0, vec4(1.0, -3.0, 3.0, -1.0));
	setRow(Bspline,1, vec4(4.0,  0.0,-6.0,  3.0));
	setRow(Bspline,2, vec4(1.0,  3.0, 3.0, -3.0));
	setRow(Bspline,3, vec4(0.0, 0.0,  0.0,  1.0));
	Bspline = Bspline*1.0f / 6.0f;
	BB = inverse(Bspline*Bbezier);
	Binit = vec3(0.0, 0.0, 1.0);

	/*BB.setRow(0,           Vec4f(1.0/6.0, 0.0, 0.0, 0.0));
	BB.setRow(1, 1.0f/6.0f*Vec4f(4.0, 4.0, 2.1, 1.0));
	BB.setRow(2, 1.0f/6.0f*Vec4f(1.0, 2.0, 4.0, 4.0));
	BB.setRow(3,           Vec4f(0.0, 0.0, 0.0, 1.0/6.0)); */
	Curve R,temp;

	for (unsigned i = 0; i < P.size() - 3; i++)
	{
		//cerr << "\t>>> "<< i << endl;
		setColumn(G1,0, vec4( P[i][0],P[i][1],P[i][2], 0 ));
		setColumn(G1,1, vec4( P[i+1][0],P[i+1][1],P[i+1][2], 0 ));
		setColumn(G1,2, vec4( P[i + 2][0],P[i + 2][1],P[i + 2][2], 0 ));
		setColumn(G1,3, vec4( P[i + 3][0],P[i + 3][1],P[i + 3][2], 0 ));

		G2 = G1*BB;
		p0 = vec3(G2[0][0], G2[1][0], G2[2][0]);
		p1 = vec3(G2[0][1], G2[1][1], G2[2][1]);
		p2 = vec3(G2[0][2], G2[1][2], G2[2][2]);
		p3 = vec3(G2[0][3], G2[1][3], G2[2][3]);
		if (adaptive)
			temp = coreBezier(p0,p1,p2,p3,Binit,0,errorbound,minstep);
		else
			temp = coreBezier(p0, p1, p2, p3, Binit, steps);
		Binit = temp[temp.size()-1].B;

		for (unsigned j = 0; j < temp.size() - 1; j++)
		{
			R.push_back(temp[j]);
			//cerr << temp[j].V[0] << " " << temp[j].V[1] << " " << temp[j].V[2] << endl;
		}



	}
	R.push_back(temp[temp.size()-1]);
	if (approx(R[0].V, R[R.size() - 1].V) && !approx(R[0].B, R[R.size() - 1].B))
		quicFixNormals(R);


	/*
    cerr << "\t>>> evalBSpline has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vec3f >): "<< endl;
    //for (unsigned i = 0; i < P.size(); ++i) {
    //    cerr << "\t>>> "; printTranspose(P[i]); cerr << endl;
    //}

    cerr << "\t>>> Steps (type steps): " << steps << endl;
    cerr << "\t>>> Returning empty curve." << endl;
	*/
    // Return an empty curve right now.
    return R;
}

Curve evalCatmullRom(const vector<vec3>& P, unsigned steps, bool adaptive, float errorbound, float minstep)
{
	// Check
	if (P.size() < 4) {
		cerr << "evalBspline must be called with 4 or more control points." << endl;
		exit(0);
	}


	//theoretically this easy:
	//according to web t=0,t=1 varies between p1 and p2 as it should...
	//works only for uniform spacing of control points....)


	mat4 Bbezier, BCR, BB, G1, G2;
	vec3 p0, p1, p2, p3, Binit;

	setRow(Bbezier,0, vec4(1.0, -3.0, 3.0, -1.0));
	setRow(Bbezier,1, vec4(0.0, 3.0, -6.0, 3.0));
	setRow(Bbezier,2, vec4(0.0, 0.0, 3.0, -3.0));
	setRow(Bbezier,3, vec4(0.0, 0.0, 0.0, 1.0));

	setRow(BCR,0, vec4(0.0, -1.0, 2.0, -1.0));
	setRow(BCR,1, vec4(2.0, 0.0, -5.0, 3.0));
	setRow(BCR,2, vec4(0.0, 1.0, 4.0, -3.0));
	setRow(BCR,3, vec4(0.0, 0.0, -1.0, 1.0));
	BCR = BCR*1.0f / 2.0f;
	BB = inverse(BCR*Bbezier);
	Binit = vec3(0.0, 0.0, 1.0);

	Curve R, temp;

	for (unsigned i = 0; i < P.size() - 3; i++)
	{
		setColumn(G1,0, vec4( P[i][0],P[i][1],P[i][2], 0 ));
		setColumn(G1,1, vec4( P[i + 1][0],P[i + 1][1],P[i + 1][2], 0 ));
		setColumn(G1,2, vec4( P[i + 2][0],P[i + 2][1],P[i + 2][2], 0 ));
		setColumn(G1,3, vec4( P[i + 3][0],P[i + 3][1],P[i + 3][2], 0 ));

		G2 = G1*BB;
		p0 = vec3(G2[0][0], G2[1][0], G2[2][0]);
		p1 = vec3(G2[0][1], G2[1][1], G2[2][1]);
		p2 = vec3(G2[0][2], G2[1][2], G2[2][2]);
		p3 = vec3(G2[0][3], G2[1][3], G2[2][3]);
		if (adaptive)
			temp = coreBezier(p0, p1, p2, p3, Binit, 0, errorbound, minstep);
		else
			temp = coreBezier(p0, p1, p2, p3, Binit, steps);
		Binit = temp[temp.size() - 1].B;

		for (unsigned j = 0; j < temp.size() - 1; j++)
		{
			R.push_back(temp[j]);
		}

	}
	if (approx(R[0].V, R[R.size() - 1].V) && !approx(R[0].B, R[R.size() - 1].B))
		quicFixNormals(R);

	return R;

}

Curve evalCircle(float radius, unsigned steps) {
    // This is a sample function on how to properly initialize a Curve
    // (which is a vector<CurvePoint>).

    // Preallocate a curve with steps+1 CurvePoints
    Curve R(steps+1);

    // Fill it in counterclockwise
    for (unsigned i = 0; i <= steps; ++i) {
        // step from 0 to 2pi
        float t = 2.0f * (float)M_PI * float(i) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        R[i].V = radius * vec3(cos(t), sin(t), 0);

        // Tangent vector is first derivative
        R[i].T = vec3(-sin(t), cos(t), 0);

        // Normal vector is second derivative
        R[i].N = vec3(-cos(t), -sin(t), 0);

        // Finally, binormal is facing up.
        R[i].B = vec3(0, 0, 1);

		//curvature is second derivative of position
		R[i].K = vec3(-cos(t), -sin(t), 0);
    }

    return R;
}

Curve evalTrefoilKnot(unsigned steps, float size) {
	// This is a sample function on how to properly initialize a Curve
	// (which is a vector<CurvePoint>).

	// Preallocate a curve with steps+1 CurvePoints
	Curve R(steps + 1);

	// Fill it in counterclockwise
	for (unsigned i = 0; i <= steps; ++i) {
		// step from 0 to 2pi
		float t = 2.0f * (float)M_PI * float(i) / steps;

		// Initialize position
		// We're pivoting counterclockwise around the y-axis
		R[i].V[0] = (sin(t) + 2.0f*sin(2.0f*t))*size;
		R[i].V[1] = (cos(t) - 2.0f*cos(2.0f*t))*size;
		R[i].V[2] = (-sin(3.0f*t))*size;

		// Tangent vector is first derivative
		R[i].T[0] = cos(t) + 4.0f*cos(2.0f*t);
		R[i].T[1] = -sin(t) + 4.0f*sin(2.0f*t);
		R[i].T[2] = -3.0f*cos(3.0f*t);
		R[i].T= normalize(R[i].T);

		// Normal vector is second derivative
		R[i].N[0] = -sin(t) -8.0f*sin(2.0f*t);
		R[i].N[1] = -cos(t) + 8.0f*cos(2.0f*t);
		R[i].N[2] = 9.0f*sin(3.0f*t);

		//curvature is the same as not normalized normal
		R[i].K[0] = R[i].N[0];
		R[i].K[1] = R[i].N[1];
		R[i].K[2] = R[i].N[2];
		//like this just in case, because of pointer and such (not sure how Vec3f handles =operator);

		//lets normalize the normal
		R[i].N=normalize(R[i].N);

		// Finally, binormal is orthogonal to N and T.
		R[i].B = normalize(cross(R[i].T,R[i].N));
	}

	return R;
}

/*
void drawCurve(const Curve& curve, float framesize) {
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // Setup for line drawing
    glDisable(GL_LIGHTING);

	// Color for the line
    glColor4f(1, 1, 1, 1);
    glLineWidth(1);

	if (framesize >= 0) {
		// Draw curve
		glBegin(GL_LINE_STRIP);
		for (unsigned i = 0; i < curve.size(); ++i) {
			glVertex(curve[i].V);
		}
		glEnd();
	}

    glLineWidth(1);

    // Draw coordinate frames if framesize nonzero
    if (framesize != 0.0f) {
		framesize = abs(framesize);
        mat4 M;

        for (unsigned i = 0; i < curve.size(); ++i) {
            M.setCol( 0, Vec4f( curve[i].N, 0 ) );
            M.setCol( 1, Vec4f( curve[i].B, 0 ) );
            M.setCol( 2, Vec4f( curve[i].T, 0 ) );
            M.setCol( 3, Vec4f( curve[i].V, 1 ) );

            glPushMatrix();
            glMultMatrixf(M.getPtr());
            glScaled(framesize, framesize, framesize);
            glBegin(GL_LINES);
            glColor3f( 1, 0, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 1, 0, 0 );
            glColor3f( 0, 1, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 1, 0 );
            glColor3f( 0, 0, 1 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 0, 1 );
            glEnd();
            glPopMatrix();
        }
    }

    // Pop state
    glPopAttrib();
}
*/
