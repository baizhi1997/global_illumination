#define __HEADER_PLANE__
#define __HEADER_PLANE__

#include "vector.h"
#include "object.h"

class Plane : public Object
{
private:
    Vector a, b, c, d;

public:	
	Plane(void);
	
    Plane(Vector pa, Vector pb, Vector pc, Vector pd, Color col)
		: a(pa), b(pb), c(pc), d(pd)
	{
		color = col;
	};

	bool isInside(Vector pos);
	
	float intersect(Vector pos, Vector dir);
	
	Vector normal(Vector pos);

};

#endif //__HEADER_PLANE__