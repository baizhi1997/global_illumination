#define __HEADER_COLOR__
#define __HEADER_COLOR__

class Color
{

public:
    float r, g, b;
	static const Color WHITE;
	static const Color BLACK;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color GRAY;
    	static const Color YELLOW;
	static const Color MAGENTA;
	static const Color PURPLE;

    Color() : r(1), g(1), b(1) {};
	
    Color(float rCol, float gCol, float bCol) : r(rCol), g(gCol), b(bCol) {};

    void scaleColor(float);

    void combineColor(Color);

    void combineColor(Color, float);

	Color phongLight(Color, float,  float);
};
#endif //__HEADER_COLOR__
