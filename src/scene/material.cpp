#include "ray.h"
#include "material.h"
#include "light.h"

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.

	vec3f Iphong;

	Iphong = ke + prod(ka, scene->getIa()); // first 2 terms of the formula

	vec3f P = r.at(i.t); // point of intersection
	for (list<Light*>::const_iterator j = scene->beginLights(); j != scene->endLights(); j++) {
		vec3f attenuation = (*j)->distanceAttenuation(P) * (*j)->shadowAttenuation(P); // TODO: distance and shadow atten
		
		vec3f L = (*j)->getDirection(P); // light direction
		vec3f diffuse = kd * maximum(i.N * L, 0); // ??

		vec3f R = (2 * (i.N * L) * i.N) - L; // reflection
		R = R.normalize();
		vec3f specular = ks * (pow(maximum(R * (-r.getDirection()), 0), shininess * 128.0));

		Iphong += prod(attenuation, diffuse + specular);
	}

	Iphong = Iphong.clamp();

	return Iphong;
}
