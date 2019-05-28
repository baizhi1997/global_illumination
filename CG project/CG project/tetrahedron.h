#define __HEADER_TETRAHEDRON__
#define __HEADER_TETRAHEDRON__

#include "vector.h"
#include "object.h"

class Tetrahedron : public Object
{
private:
    Vector a, b, c;

public:	
	Tetrahedron(void);
	
    Tetrahedron (Vector pa, Vector pb, Vector pc, Color col)
		: a(pa), b(pb), c(pc)
	{
		color = col;
	};

	bool isInside(Vector pos);
	
	float intersect(Vector pos, Vector dir);
	
	Vector normal(Vector pos);

};
#endif //__HEADER_TETRAHEDRON__