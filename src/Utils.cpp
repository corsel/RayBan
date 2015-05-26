#include "Utils.h"

float ambientLuminance = 1.0f;

//Vector3 Struct
Vector3::Vector3(float argX, float argY, float argZ)
: x(argX), y(argY), z(argZ) {}
Vector3 Vector3::operator+(Vector3 argVector)
{
	return Vector3(x + argVector.x, y + argVector.y, z + argVector.z);
}
Vector3 Vector3::operator-(Vector3 argVector)
{
	return Vector3(x - argVector.x, y - argVector.y, z - argVector.z);
}
Vector3 Vector3::scalarMultiply(Vector3 argVector, float argScalar)
{
	return Vector3(argVector.x*argScalar, argVector.y*argScalar, argVector.z*argScalar);
}
float Vector3::dot(Vector3 argVector0, Vector3 argVector1) //static
{
	return argVector0.x*argVector1.x + argVector0.y*argVector1.y + argVector0.z*argVector1.z;
}
float Vector3::getLength()
{
	return sqrt(x*x + y*y + z*z);
}

//Color Struct
Color::Color()
: red(0.0f), green(0.0f), blue(0.0f) {}
Color::Color(float argRed, float argGreen, float argBlue)
: red(argRed), green(argGreen), blue(argBlue) {}
Color Color::scalarMultiply(Color argColor, float argScalar)
{
	return Color(argColor.red*argScalar, argColor.green*argScalar, argColor.blue*argScalar);
}

//Ray Struct
Ray::Ray(Vector3 argStart, Vector3 argEnd)
: startPoint(argStart), endPoint(argEnd) 
{
	f = startPoint;
	e = endPoint - f;
}
float Ray::getLength()
{
	return sqrt(startPoint.x*startPoint.x + startPoint.y*startPoint.y + startPoint.z*startPoint.z);
}

//Object Base Struct
Object::Object() {}
Object::Object(Color argDiffuse, float argMaterialCoeff)
: diffuseColor(argDiffuse), materialCoeff(argMaterialCoeff) {}

//Triangle Struct
Triangle::Triangle(Vector3 argPoint0, Vector3 argPoint1, Vector3 argPoint2, Color argDiffuse, float argMaterialCoeff)
: Object(argDiffuse, argMaterialCoeff)
{
	diffuseColor = argDiffuse;
	point[0] = argPoint0;
	point[1] = argPoint1;
	point[2] = argPoint2;
	c = point[0];
	b = point[2] - c;
	a = point[1] - c;
	objectVector.push_back(this);
	Vector3 defaultNormalVector(MathUtils::arrayCrossProduct(b, a));
}
Triangle::Triangle(Vector3 argPoint0, Vector3 argPoint1, Vector3 argPoint2, Vector3 argNormal0, Vector3 argNormal1, Vector3 argNormal2, Color argDiffuse, float argMaterialCoeff)
: Object(argDiffuse, argMaterialCoeff)
{
	diffuseColor = argDiffuse;
	point[0] = argPoint0;
	point[1] = argPoint1;
	point[2] = argPoint2;
	c = point[0];
	b = point[2] - c;
	a = point[1] - c;
	objectVector.push_back(this);
	normal[0] = argNormal0;
	normal[1] = argNormal1;
	normal[2] = argNormal2;
}
float Triangle::getHitValue(Ray argRay)
{
	float *temp = MathUtils::rayTriangleIntersection(argRay, *this);
	if (temp[0] < 0.0f || temp[1] < 0.0f || temp[0] + temp[1] > 1.0f) //hit is out of triangle
	{
		delete[] temp;
		return -1.0f;
	}
	float param = temp[2];
	delete[] temp;
	return param;
}
Color Triangle::getColor(Ray argRay)
{
	Color returnColor;
	returnColor = Color::scalarMultiply(diffuseColor, ambientLuminance);

	return returnColor;
}

//Sphere Struct
Sphere::Sphere(Vector3 argCenter, float argRadius, Color argDiffuse, float argMaterialCoeff)
: Object(argDiffuse, argMaterialCoeff), center(argCenter), radius(argRadius)
{
	diffuseColor = argDiffuse;
	objectVector.push_back(this);
}
float Sphere::getHitValue(Ray argRay)
{
	return 99.9f;
}
Color Sphere::getColor(Ray argRay)
{
	return Color();
}

std::vector<Object*> objectVector;
std::vector<Light*> lightVector;

//Camera Class
Camera::Camera(Vector3 argPosition, Vector3 argLookAt, unsigned int argScreenWidth, unsigned int argScreenHeight, float argNearClipping, float argFarClipping, float argFieldOfView, Vector3 argUp)
: position(argPosition), screenWidth(argScreenWidth), screenHeight(argScreenHeight), nearClipping(argNearClipping), farClipping(argFarClipping), fieldOfView(argFieldOfView)
{
   pixelOffset.x = nearClipping / tan(fieldOfView / 2.0f);
   pixelSize = pixelOffset.x / float(screenWidth);
   pixelOffset.y = pixelSize * (float(screenHeight) - 1.0f) / 2.0f;
   pixelOffset.x -= pixelSize / 2.0f;
   direction = MathUtils::normalize(argLookAt - position);
   up = MathUtils::normalize(argUp);
   binormal = MathUtils::arrayCrossProduct(up, direction);
}
int Camera::getScreenWidth() {return screenWidth;}
int Camera::getScreenHeight() {return screenHeight;}
Ray Camera::getPixelRay(unsigned int argX, unsigned int argY)
{
	Vector3 pixelLocalPosn = Vector3::scalarMultiply(direction, nearClipping) + Vector3::scalarMultiply(up, -pixelOffset.y + pixelSize * float(argY)) + Vector3::scalarMultiply(binormal, -pixelOffset.x + pixelSize * float(argX));
	pixelLocalPosn = MathUtils::normalize(pixelLocalPosn);
	Vector3 farPlanePosn = Vector3::scalarMultiply(pixelLocalPosn, farClipping - nearClipping);
	return Ray(pixelLocalPosn, farPlanePosn);
}

//MathUtils Namespace
Vector3 MathUtils::normalize(Vector3 argVector)
{
   float magnitude = sqrt(argVector.x*argVector.x + argVector.y*argVector.y + argVector.z*argVector.z);
   return Vector3(argVector.x / magnitude, argVector.y / magnitude, argVector.y / magnitude);
}
float *MathUtils::rayTriangleIntersection(Ray argRay, Triangle argTriangle)
{
	float *aMatrix = new float[9];
	aMatrix[0] = argTriangle.a.x;
	aMatrix[1] = argTriangle.b.x;
	aMatrix[2] = -argRay.e.x;
	aMatrix[3] = argTriangle.a.y;
	aMatrix[4] = argTriangle.b.y;
	aMatrix[5] = -argRay.e.y;
	aMatrix[6] = argTriangle.a.z;
	aMatrix[7] = argTriangle.b.z;
	aMatrix[8] = -argRay.e.z;

	float *cArray = new float[3];
	cArray[0] = argRay.f.x - argTriangle.c.x;
	cArray[1] = argRay.f.y - argTriangle.c.y;
	cArray[2] = argRay.f.z - argTriangle.c.z;

	float *uvtArray = solveLinear(aMatrix, cArray);
	//std::cout << "u: " << uvtArray[0] << ", v: " << uvtArray[1] << ", t: " << uvtArray[2] << std::endl; //debug

	delete[] aMatrix, cArray;
	return uvtArray;
}
float MathUtils::getDeterminant3x3(float *argMatrix)
{
	return (argMatrix[2] * argMatrix[3] * argMatrix[7] + argMatrix[1] * argMatrix[5] * argMatrix[6] + argMatrix[0] * argMatrix[4] * argMatrix[8] - (argMatrix[2] * argMatrix[4] * argMatrix[6] + argMatrix[1] * argMatrix[3] * argMatrix[8] + argMatrix[0] * argMatrix[5] * argMatrix[7]));
}
float *MathUtils::solveLinear(float *argA, float *argC)
{
	float *tempMatrix = new float[9];
	float *returnArray = new float[3];
	float determinant = getDeterminant3x3(argA);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (j % 3 == i)
				tempMatrix[j] = argC[j / 3];
			else
				tempMatrix[j] = argA[j];
		}
		returnArray[i] = getDeterminant3x3(tempMatrix) / determinant;
	}
	delete[] tempMatrix;
	return returnArray;
}
Vector3 MathUtils::arrayCrossProduct(Vector3 argArray0, Vector3 argArray1)
{
	return Vector3(argArray0.y*argArray1.z - argArray0.z*argArray1.y,
		argArray0.z*argArray1.x - argArray0.x*argArray1.z,
		argArray0.x*argArray1.y - argArray0.y*argArray1.x);
}
float MathUtils::getCosProjectionAngle(Vector3 argVector0, Vector3 argVector1)
{
	float dotProduct = Vector3::dot(argVector0, argVector1);
	dotProduct /= argVector0.getLength();
	dotProduct /= argVector1.getLength();
	return dotProduct;
}

//Operation Namespace
int Operations::findNearestHit(Ray argRay)
{
	float nearestDistance = 1.0f;
	int nearestIndex = -1;
	for (unsigned int i = 0; i < objectVector.size(); i++)
	{
		float distance = objectVector[i]->getHitValue(argRay);
		if (distance < 0.0f) continue;
		if (distance < nearestDistance)
		{
			nearestIndex = i;
			nearestDistance = distance;
		}
	}
	return nearestIndex;
}