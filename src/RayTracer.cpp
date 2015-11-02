// The main ray tracer.

#include <Fl/fl_ask.h>

#include "RayTracer.h"
#include "global.h"
#include "ui/TraceUI.h"

#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"

#include "fileio/read.h"
#include "fileio/parse.h"

#define 	M_PI   3.14159265358979323846	/* pi */

// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
    ray r( vec3f(0,0,0), vec3f(0,0,0) );
    scene->getCamera()->rayThrough( x,y,r );
	return traceRay(scene, r, vec3f(1.0, 1.0, 1.0), traceUI->getDepth()).clamp();
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay( Scene *scene, const ray& r, const vec3f& thresh, int depth )
{
	isect i;
	vec3f colorC;
	//scene->useKdTree = traceUI->m_kdTree;
	// cout<<scene->useKdTree<<endl;
	if (scene->intersect(r, i)) {
		// YOUR CODE HERE

		// An intersection occurred!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.
		const Material& m = i.getMaterial();
		// Light Ray
		vec3f intensity = m.shade(scene, r, i);
		if (depth == 0)
		{
			return intensity;
		}
		vec3f Qpt = r.at(i.t);
		vec3f minusD = -1 * r.getDirection();
		vec3f cosVector = i.N * (minusD * i.N);
		vec3f sinVector = cosVector + r.getDirection();
		// Reflected Ray
		if (!m.kr(i).iszero())
		{
			vec3f reflectedDirection = cosVector + sinVector;
			reflectedDirection.normalize();
			ray reflectedRay(Qpt, reflectedDirection);
			intensity = intensity + prod(m.kr(i), traceRay(scene, reflectedRay, thresh, depth - 1));
		}
		//Refracted Ray
		if (!m.kt(i).iszero())
		{
			double cosineAngle = acos(i.N * r.getDirection()) * 180 / M_PI;
			double n_i, n_r;
			double criticalAngle = 360;
			int iDirection;
			// bool goingIn = true;
			// double cosThetaI = 0;
			if (cosineAngle > 90) // Coming into an object from air
			{
				n_i = 1;
				n_r = m.index(i);
				iDirection = 1;
				// cosThetaI = i.N * -1 * r.d;
			}
			else // Going out from object to air
			{
				n_i = m.index(i);
				n_r = 1;
				// goingIn = false;
				// cosThetaI = i.N * r.d;
				iDirection = -1;
			}
			double n = n_i / n_r;
			vec3f sinT = (n_i / n_r) * sinVector;
			vec3f cosT = (-1 * i.N) * sqrt(1 - sinT*sinT);
			if (cosineAngle < criticalAngle)
			{
				vec3f refractedDirection = cosT + iDirection*sinT;
				refractedDirection.normalize();
				ray refractedRay(Qpt, iDirection * refractedDirection);
				intensity = intensity + prod(m.kt(i), traceRay(scene, refractedRay, thresh, depth - 1));
			}
		}
		colorC = intensity;
	}
	else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.
		colorC = vec3f (0.0, 0.0, 0.0);
	}
	return colorC;
}

RayTracer::RayTracer()
{
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;

	m_bSceneLoaded = false;
}


RayTracer::~RayTracer()
{
	delete [] buffer;
	delete scene;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];
	
	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];
	}
	memset( buffer, 0, w*h*3 );
}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
			tracePixel(i,j);
}

void RayTracer::tracePixel( int i, int j )
{
	vec3f col;

	if( !scene )
		return;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	col = trace( scene,x,y );

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}