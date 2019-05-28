#include "pentagonal_pyramid.h"
#include "vector.h"
#include <math.h>

bool Pentagonal_pyramid::isInside(Vector q)
{
	Vector n = normal(q);
	Vector ua = b - a, ub = c - b, uc = a - c;
	Vector va = q - a, vb = q - b, vc = q - c;

	if ((ua.cross(va).dot(n) > 0) and (ub.cross(vb).dot(n) > 0) and (uc.cross(vc).dot(n) > 0)) {
		return true;
	}
	else {
		return false;
	}

}

float Pentagonal_pyramid::intersect(Vector pos, Vector dir)
{
	Vector n = normal(pos);
	Vector vdif = a - pos;
	float vdotn = dir.dot(n);
	if (fabs(vdotn) < 1.e-4) return -1;
	float t = vdif.dot(n) / vdotn;
	if (fabs(t) < 0.0001) return -1;
	Vector q = pos + dir * t;
	if (isInside(q)) return t;
	else return -1;
}



Vector Pentagonal_pyramid::normal(Vector pos)
{
	Vector result = (b - a).cross(c - a);
	result.normalise();
	return result;
}
