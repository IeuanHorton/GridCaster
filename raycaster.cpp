#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#include "player.h"
#include "keybindings.h"
#include "settings.h"
#include "map.h"

#define PI 3.1415926535

Map map;

void drawPlayer()
{
	glColor3f(1,1,0); //Yellow
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(pX,pY);
	glEnd();

	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(pX,pY);
	glVertex2i(pX+pDeltaX*5,pY+pDeltaY*5);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	map.draw2DMap();
	drawPlayer();
	glutSwapBuffers();
}

void buttons(unsigned char key, int x, int y)
{
	if(key == LEFT)
	{
		pAngle -= TURNSPEED;
	}

	if(key == RIGHT)
	{
		pAngle += TURNSPEED;
	}

	if(pAngle > 2*PI || pAngle < 0)
	{
		pAngle+=2*PI;
	}

	pDeltaX=cos(pAngle)*5;
	pDeltaY=sin(pAngle)*5;

	if(key == UP)
	{
		pX += pDeltaX;
		pY += pDeltaY;
	}
	if(key == DOWN)
	{
		pX -= pDeltaX;
		pY -= pDeltaY;
	}

	glutPostRedisplay();
}

void init()
{
	glClearColor(0.3,0.3,0.3,0);
	gluOrtho2D(0,WINDOWWIDTH,WINDOWHEIGHT,0);
	pX=300; pY=300;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOWWIDTH,WINDOWHEIGHT);
	glutCreateWindow("RAYCASTERTEST");
	init();
	glutDisplayFunc(display);
        glutKeyboardFunc(buttons);
	glutMainLoop();
}
