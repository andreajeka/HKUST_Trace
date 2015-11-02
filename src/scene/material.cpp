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

	// the diffuse and ambient terms are multiplied by (1-kt) as advised by the doc
	vec3f Iphong = ke(i) + prod(vec3f(1, 1, 1) - kt(i), prod(ka(i), scene->getIa())); // first 2 terms of the formula
	vec3f P = r.at(i.t); // point of intersection

	for (list<Light*>::const_iterator j = scene->beginLights(); j != scene->endLights(); j++) {
		vec3f attenuation = (*j)->distanceAttenuation(P) * (*j)->shadowAttenuation(P);
		
		vec3f L = (*j)->getDirection(P); // light direction
		vec3f diffuse = prod(vec3f(1, 1, 1) - kt(i), kd(i) * maximum(i.N * L, 0));

		vec3f R = (2 * (i.N * L) * i.N) - L; // reflection
		R = R.normalize();
		vec3f specular = ks(i) * (pow(maximum(R * (-r.getDirection()), 0), shininess(i) * 128.0));

		Iphong += prod(attenuation, diffuse + specular);
	}

	Iphong = Iphong.clamp();

	return Iphong;

	//vec3f Qpoint = r.at(i.t);
	//vec3f intensity = ke(i) + prod(ka(i), scene->ambient());

	//for (vector<Light*>::const_iterator litr = scene->beginLights(); litr != scene->endLights(); ++litr)
	//{
	//	if (kd(i).iszero() && ks(i).iszero())
	//	{
	//		continue;
	//	}
	//	Light* pLight = *litr;
	//	// Diffuse Term
	//	vec3f directionToLight = pLight->getDirection(Qpoint);
	//	directionToLight.normalize();
	//	vec3f lightIntensity = pLight->distanceAttenuation(Qpoint) * pLight->shadowAttenuation(r, Qpoint);
	//	if (!kd(i).iszero())
	//	{
	//		intensity = intensity + prod(kd(i), lightIntensity) * max((i.N * directionToLight), 0.0);
	//	}
	//	// Specular term
	//	if (!ks(i).iszero())
	//	{
	//		vec3f viewingDirection = scene->getCamera().getEye() - Qpoint;
	//		viewingDirection.normalize();
	//		vec3f cosVector = i.N * (directionToLight * i.N);
	//		vec3f sinVector = cosVector - directionToLight;
	//		vec3f reflectedDirection = cosVector + sinVector;
	//		reflectedDirection.normalize();
	//		intensity = intensity + prod(ks(i), lightIntensity) * pow(max(reflectedDirection*viewingDirection, 0.0), shininess(i));
	//	}
	//}
	//return intensity;
}

vec3f MaterialParameter::value(const isect& is) const
{
	return _value;
}

double MaterialParameter::intensityValue(const isect& is) const
{
	return (0.299 * _value[0]) + (0.587 * _value[1]) + (0.114 * _value[2]);
}
