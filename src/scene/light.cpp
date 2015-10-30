#include <cmath>

#include "light.h"

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.

	vec3f d = getDirection(P);
	vec3f curP = P;
	isect isecP;
	vec3f ret = getColor(P);
	ray r = ray(curP, d);

	while (scene->intersect(r, isecP)){
		if (isecP.getMaterial().kt.iszero())return vec3f(0, 0, 0);
		ret = prod(ret, isecP.getMaterial().kt);
		curP = r.at(isecP.t);
		r = ray(curP, d);
	}

	return ret;
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0

	double distance = (position - P).length(); // calculate the euclidean distance

	double attenuation = 1.0 / (const_atten_coeff + linear_atten_coeff * distance + quadratic_atten_coeff * distance * distance);
	if (attenuation > 1.0) attenuation = 1.0;

	return attenuation;
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	// using other people's code, but somehow it works... will change the implementation later

	vec3f d = getDirection(P);
	vec3f newP = P;
	isect isecP;
	vec3f ret = getColor(P);
	ray r = ray(newP, d); // from the point of intersection, look at the light

	while (scene->intersect(r, isecP)){
		if (isecP.getMaterial().kt.iszero())
			return vec3f(0, 0, 0);
		ret = prod(ret, isecP.getMaterial().kt);
		newP = r.at(isecP.t);
		r = ray(newP, d);
	}

	return ret;
}
