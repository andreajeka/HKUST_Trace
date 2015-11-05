#include <cmath>
#include <algorithm>

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

	vec3f d = getDirection(P); // direction from the point to be shaded towards the light source
	vec3f p = P + d * RAY_EPSILON; // point to be shaded
	ray r = ray(p, d, ray::SHADOW); // from the point of intersection, look at the light
	
	isect isecSR; // intersection of the shadow ray
	vec3f intensity = vec3f(1.0, 1.0, 1.0); // colour of light source

	while (scene->intersect(r, isecSR)) { // if the ray intersect with an object
		if (isecSR.getMaterial().kt(isecSR).iszero()) { // if the material of the object is opaque
			return vec3f(0, 0, 0); // no shadow if opaque
		}
		else { // if transmissive
			intensity = prod(intensity, isecSR.getMaterial().kt(isecSR)); // close to zero -> lower value
			p = r.at(isecSR.t) + d * RAY_EPSILON; // to find the next closest intersection
			r = ray(p, d, ray::SHADOW);
		}
	}

	return intensity;

	//vec3f d = getDirection(P);
	//d.normalize();
	//isect  isecSR;
	//ray r(P, d, ray::SHADOW);
	//if (this->getScene()->intersect(r, isecSR))  // if the ray intersect with an object
	//{
	//	return vec3f(0, 0, 0); // no shadow if opaque
	//}
	//return color; // return colour of light source
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

	vec3f d = getDirection(P); // direction from the point to be shaded towards the light source
	vec3f p = P + d * RAY_EPSILON; // point to be shaded
	ray r = ray(p, d, ray::SHADOW); // from the point of intersection, look at the light

	isect isecSR; // intersection of the shadow ray
	vec3f intensity = vec3f(1.0, 1.0, 1.0); // colour of light source

	while (scene->intersect(r, isecSR)) { // if the ray intersect with an object
		if (isecSR.getMaterial().kt(isecSR).iszero()) { // if the material of the object is opaque
			return vec3f(0, 0, 0); // no shadow if opaque
		}
		else { // if transmissive
			intensity = prod(intensity, isecSR.getMaterial().kt(isecSR)); // close to zero -> lower value
			p = r.at(isecSR.t) + d * RAY_EPSILON; // to find the next closest intersection
			r = ray(p, d, ray::SHADOW);
		}
	}

	return intensity;

	//vec3f d = getDirection(P);
	//d.normalize();
	//isect  isecSR;
	//ray r(P, d, ray::SHADOW);
	//if (this->getScene()->intersect(r, isecSR))  // if the ray intersect with an object
	//{
	//	return vec3f(0, 0, 0); // no shadow if opaque
	//}
	//return color; // return colour of light source
}
