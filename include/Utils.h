#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <iostream>
#include <math.h>
#include <vector>

extern float ambientLuminance;

struct Vector3
{
	float x, y, z;

	Vector3(float argX = 0.0f, float argY = 0.0f, float argZ = 0.0f);
	Vector3 operator+(Vector3 argVector);
	Vector3 operator-(Vector3 argVector);
	static Vector3 scalarMultiply(Vector3 argVector, float argScalar);
	static float dot(Vector3 argVector0, Vector3 argVector1);
	float getLength(void);
};
struct Color
{
	float red, green, blue;

	Color(void);
	Color(float argRed, float argGreen, float argBlue);
	static Color scalarMultiply(Color argColor, float argScalar);
};
struct Ray
{
	/*
	Ray is parametrized as:
	e_x * t + f_x = x
	e_y * t + f_y = y
	e_z * t + f_z = z

	startPoint is assumed t=0
	endPoint is assumed t=1
	*/
	Vector3 startPoint, endPoint;
	Vector3 e, f;

	Ray(Vector3 argStart, Vector3 argEnd);
	float getLength(void);
};
struct Object //abstract
{
	Color diffuseColor;
	float materialCoeff;

	Object(void);
	Object(Color argDiffuse, float argMaterialCoeff);
	virtual float getHitValue(Ray argRay) = 0;
	virtual Color getColor(Ray argRay) = 0;
};
struct Triangle : public Object
{
	/*
	Triangle is parametrized as:
	a_x * u + b_x * v + c_x = x
	a_y * u + b_y * v + c_y = y
	a_z * u + b_z * v + c_z = z

	point[0] is assumed u=0, v=0
	point[1] is assumed u=1, v=0
	point[2] is assumed u=0, v=1
	*/
	Vector3 point[3];
	Vector3 normal[3];
	Vector3 a, b, c;

	Triangle(Vector3 argPoint0, Vector3 argPoint1, Vector3 argPoint2, Color argDiffuse = Color(1.0f, 0.0f, 0.0f), float argMaterialCoeff = 1.0f);
	Triangle(Vector3 argPoint0, Vector3 argPoint1, Vector3 argPoint2, Vector3 argNormal0, Vector3 argNormal1, Vector3 argNormal2, Color argDiffuse = Color(1.0f, 0.0f, 0.0f), float argMaterialCoeff = 1.0f);
	float getHitValue(Ray argRay); //virtual implementation; returns t of ray if hit, -1 if not hit
	Color getColor(Ray argRay); //virtual implementation
};
struct Sphere : public Object
{
	Vector3 center;
	float radius;

	Sphere(Vector3 argCenter, float argRadius, Color argDiffuse = Color(0.0f, 0.0f, 1.0f), float argMaterialCoeff = 1.0f);
	float getHitValue(Ray argRay); //virtual implementation
	Color getColor(Ray argRay); //virtual implementation
};
struct Light
{

};

extern std::vector<Object*> objectVector;
extern std::vector<Light*> lightVector;

class Camera
{
private:
	Vector3 position;
	Vector3 direction;
	Vector3 up;
	Vector3 binormal;
	unsigned int	screenWidth,
					screenHeight;
	float	nearClipping,
			farClipping;
	float fieldOfView;

	Vector3 pixelOffset;
	float	pixelSize;

public:
	Camera(void);
	Camera(Vector3 argPosition, Vector3 argLookAt, unsigned int argScreenWidth, unsigned int argScreenHeight, float argNearClipping, float argFarClipping, float argFieldOfView, Vector3 argUp = Vector3(0.0f, 0.0f, 1.0f));
	int getScreenWidth(void);
	int getScreenHeight(void);
	Ray getPixelRay(unsigned int argX, unsigned int argY); //Shoots a ray from the camera position to the pixel determined by argX and argY
};

namespace MathUtils
{
	Vector3 normalize(Vector3 argVector);
	float *rayTriangleIntersection(Ray argRay, Triangle argPlane); //Solves the intersection of given triangle and ray for their u, v and t values.
	float getDeterminant3x3(float *argMatrix);
	float *solveLinear(float *argA, float *argC); //Solves linear system A*x = c by Cramer's Rule.
	Vector3 arrayCrossProduct(Vector3 argArray0, Vector3 argArray1);
}

namespace Operations
{
	int findNearestHit(Ray argRay); //Returns the index of nearest hit object. Returns -1 if none is hit. 
}
#endif //UTILS_H_INCLUDED
