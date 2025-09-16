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
	glVertex2i(playerX,playerY);
	glEnd();

	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(playerX,playerY);
	glVertex2i(playerX+playerDeltaX*5,playerY+playerDeltaY*5);
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
		playerAngle -= TURNSPEED;
	}

	if(key == RIGHT)
	{
		playerAngle += TURNSPEED;
	}

	if(playerAngle > 2*PI || playerAngle < 0)//Resets the angle is it goes over 360 or under 0
	{
		playerAngle+=2*PI;
	}

	playerDeltaX=cos(pAngle)*5;
	playerDeltaY=sin(pAngle)*5;

	if(key == UP)
	{
		playerX += playerDeltaX;
		playerY += playerDeltaY;
	}
	if(key == DOWN)
	{
		playerX -= playerDeltaX;
		playerY -= playerDeltaY;
	}

	glutPostRedisplay();
}

void init()
{
	glClearColor(0.3,0.3,0.3,0);
	gluOrtho2D(0,WINDOWWIDTH,WINDOWHEIGHT,0);
	playerX=300; playerY=300;
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
