#include <cmath>
#include <assert.h>

#include "Box.h"

bool Box::intersectLocal( const ray& r, isect& i ) const {
	double Tnear = -INFINITY, Tfar = INFINITY;
	double size = 0.5;
	vec3f Xo = r.getPosition(), Xd = r.getDirection();

	// check if the light ray pass through the bounding box
	for (int j = 0; j < 3; j++) {
		if (Xd[j] == 0) {
			if (Xo[j] < -size || Xo[j] > size)
				return false;
		}
		else {
			double T1 = (-size - Xo[j]) / Xd[j];
			double T2 = (size - Xo[j]) / Xd[j];
			if (T1 > T2) /* since T1 intersection with near plane */
				swap(T1, T2);
			if (T1 > Tnear) /* want largest Tnear */
				Tnear = T1;
			if (T2 < Tfar) /* want smallest Tfar */
				Tfar = T2;
			if (Tnear > Tfar || Tfar < RAY_EPSILON)
				return false;
		}
	}

	i.obj = this;
	i.t = Tnear;

	vec3f isectP = r.at(i.t);
	for (int j = 0; j < 3; j++) {
		if (abs(isectP[j] - (-size)) < RAY_EPSILON) {
			i.N = vec3f(-(float)(j == 0), -(float)(j == 1), -(float)(j == 2));
			return true;
		}
		else if (abs(isectP[j] - size) < RAY_EPSILON) {
			i.N = vec3f((float)(j == 0), (float)(j == 1), (float)(j == 2));
			return true;
		}
	}

	return false;
}
