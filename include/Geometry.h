#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED
struct Vector3
{
	float x, y, z;

	Vector3(float argX = 0.0f, float argY = 0.0f, float argZ = 0.0f);
};
class Ray
{
private:
	Vector3 startPoint;
	Vector3 endPoint;

public:
	Ray(void);
	Ray(Vector3 argStart, Vector3 argEnd);
};

class Shape //abstract
{

};
class Triangle : public Shape
{
private:
	Vector3 vertex[3];
	Vector3 normal[3];

public:
	Triangle(Vector3 argVertex0, Vector3 argVertex1, Vector3 argVertex2);
};
class Sphere : public Shape
{
private:
	Vector3 center;
	float radius;

public:
	Sphere(Vector3 argCenter, float argRadius);
};

#endif //GEOMETRY_H_INCLUDED