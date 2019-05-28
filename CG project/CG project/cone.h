#define __HEADER_CONE__
#define __HEADER_CONE__

#include "Object.h"

class Cone : public Object
{

private:
    Vector center;
    float radius;
    float height;

public:	
	Cone()
		: center(Vector()), radius(1), height(1)
	{
		color = Color::WHITE;
	};
	
    Cone(Vector c, float r, float h, Color col)
		: center(c), radius(r), height(h)
	{
		color = col;
	};

	float intersect(Vector pos, Vector dir);

	Vector normal(Vector p);

};

#endif //__HEADER_CONE__