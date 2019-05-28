#define __HEADER_SPHERE__
#define __HEADER_SPHERE__

#include "object.h"

class Sphere : public Object
{

private:
    Vector center;
    float radius;

public:	
	Sphere()
		: center(Vector()), radius(1)
	{
		color = Color::WHITE;
	};
	
    Sphere(Vector c, float r, Color col)
		: center(c), radius(r)
	{
		color = col;
	};

	float intersect(Vector pos, Vector dir);

	Vector normal(Vector p);

};

#endif //__HEADER_SPHERE__