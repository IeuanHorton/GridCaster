#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "player.h"
#include "keybindings.h"
#include "settings.h"

void drawPlayer()
{
	glColor3f(1,1,0); //Yellow
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(pX,pY);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawPlayer();
	glutSwapBuffers();
}

void buttons(unsigned char key, int x, int y)
{
	if(key == UP)
	{
		pY-=MOVESPEED;
	}

	if(key == DOWN)
	{
		pY+=MOVESPEED;
	}

	if(key == LEFT)
	{
		pX-=MOVESPEED;
	}

	if(key == RIGHT)
	{
		pX+=MOVESPEED;
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
