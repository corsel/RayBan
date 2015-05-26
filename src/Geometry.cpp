#include "Geometry.h"

//Vector3 Struct
Vector3::Vector3(float argX, float argY, float argZ)
: x(argX), y(argY), z(argZ) {}

//Ray Class
Ray::Ray() {}
Ray::Ray(Vector3 argStart, Vector3 argEnd)
: startPoint(argStart), endPoint(argEnd) {}

//Triangle Class
Triangle::Triangle(Vector3 argVertex0, Vector3 argVertex1, Vector3 argVertex2)
{
	vertex[0] = argVertex0;
	vertex[1] = argVertex1;
	vertex[2] = argVertex2;
}

//Sphere Class
Sphere::Sphere(Vector3 argCenter, float argRadius)
: center(argCenter), radius(argRadius) {}