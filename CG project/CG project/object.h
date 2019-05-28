#define __HEADER_OBJECT__
#define __HEADER_OBJECT__

#include "Vector.h"
#include "Color.h"

class Object 
{
protected:
	Color color;
public:
	Object() {}
    virtual float intersect(Vector pos, Vector dir) = 0;
	virtual Vector normal(Vector pos) = 0;
	virtual ~Object() {}
	Color getColor();
	void setColor(Color col);
};

#endif //__HEADER_OBJECT__