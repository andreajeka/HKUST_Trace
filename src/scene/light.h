#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "scene.h"
//#include "../global.h"
//#include "../ui/TraceUI.h"
class Light
	: public SceneElement
{
public:
	virtual vec3f shadowAttenuation(const vec3f& P) const = 0;
	virtual double distanceAttenuation( const vec3f& P ) const = 0;
	virtual vec3f getColor( const vec3f& P ) const = 0;
	virtual vec3f getDirection( const vec3f& P ) const = 0;

protected:
	Light( Scene *scene, const vec3f& col )
		: SceneElement( scene ), color( col ) {}

	vec3f 		color;
};

class DirectionalLight
	: public Light
{
public:
	DirectionalLight( Scene *scene, const vec3f& orien, const vec3f& color )
		: Light( scene, color ), orientation( orien ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;

protected:
	vec3f 		orientation;
};

class PointLight
	: public Light
{
public:
	//PointLight(Scene *scene, const vec3f& pos, const vec3f& color)
	//	: Light(scene, color), position(pos)
	//{
	//	const_atten_coeff = traceUI->getConstAttenuationVal();
	//	linear_atten_coeff = traceUI->getLinearAttenuationVal();
	//	quadratic_atten_coeff = traceUI->getQuadAttenuationVal();
	//}

	PointLight( Scene *scene, const vec3f& pos, const vec3f& color, double a0 = 1.0, double a1 = 0.0, double a2 = 0.0) 
		: Light(scene, color), position(pos), const_atten_coeff(a0), linear_atten_coeff(a1), quadratic_atten_coeff(a2) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;
	
protected:
	vec3f position;
	double const_atten_coeff;
	double linear_atten_coeff;
	double quadratic_atten_coeff;
};

#endif // __LIGHT_H__
