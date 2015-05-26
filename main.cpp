#include "Utils.h"
#include "freeglut.h"
#include <iostream>

Camera *mainCam;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	for (int i = 0; i < mainCam->getScreenWidth(); i++)
	{
		for (int j = 0; j < mainCam->getScreenHeight(); j++)
		{
			Ray currentRay = mainCam->getPixelRay(i, j);
			int nearestObjectIndex = Operations::findNearestHit(currentRay);
			if (nearestObjectIndex < 0) continue;
			std::cout << "ray hit!\n";
			Color pixelColor = objectVector[nearestObjectIndex]->getColor(currentRay);
			glColor3f(pixelColor.red, pixelColor.green, pixelColor.blue);
			glVertex2i(i, j);
		}
	}
	glEnd();
	
	glFlush();
}

void shutdown()
{
	delete[] mainCam;
}

void init(int *argc, char **argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(mainCam->getScreenWidth(), mainCam->getScreenHeight());
	glutCreateWindow("RayBan");

	glutDisplayFunc(&display);
	//glutIdleFunc(&display);

	glMatrixMode(GL_PROJECTION);
	glOrtho(0, mainCam->getScreenWidth(), 0, mainCam->getScreenHeight(), 1, -1);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char **argv)
{
	Triangle triangle(Vector3(3.0f, 3.0f, -3.0f),
		Vector3(5.0f, 3.0f, -3.0f),
		Vector3(3.0f, 5.0f, -3.0f));

	mainCam = new Camera(
		Vector3(-5.0f, -5.0f, 2.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		320, 180,
		0.5f, 100.0f,
		60.0f);

	init(&argc, argv);

	glutMainLoop();
	shutdown();
}