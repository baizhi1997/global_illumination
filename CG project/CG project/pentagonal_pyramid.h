#define __pentagonal_pyramid__
#define __pentagonal_pyramid__


#include "vector.h"
#include "object.h"

class  Pentagonal_pyramid : public Object
{
private:
	Vector a, b, c;

public:
	Pentagonal_pyramid(void);

	Pentagonal_pyramid(Vector pa, Vector pb, Vector pc, Color col)
		: a(pa), b(pb), c(pc)
	{
		color = col;
	};

	bool isInside(Vector pos);

	float intersect(Vector pos, Vector dir);

	Vector normal(Vector pos);

};
#endif //__pentagonal_pyramid__