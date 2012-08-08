#include "vector.h"
class Surface
{

public:

	Surface();
        Surface(Vector v, float param);
	~Surface();

private:
	// Attributes
	Vector normal;
	float parameter;		
};

