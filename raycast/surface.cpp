#include "surface.h"
Surface::Surface()
{
	normal = {0,0,0};
	parameter = 0;
}

Surface::Surface(Vector v, float param)
{
	normal = v;
	parameter = param;
}

