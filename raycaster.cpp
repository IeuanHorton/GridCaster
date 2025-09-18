#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#include "player.h"
#include "keybindings.h"
#include "settings.h"
#include "map.h"
#include "ray.h"

#define PI 3.1415926535

Map map;
Ray ray;

int NUMOFRAYS = 1;

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

void horizontalLineCheck()
{
	int depthOfField, mapX, mapY, mapPosition;
	float yOffset, xOffset;


	depthOfField = 0;
	float aTan = -1/tan(ray.rayAngle);
	if(ray.rayAngle>PI)//Looking up
	{
		ray.rayY = (((int)playerY>>6)<<6)-0.0001;
		ray.rayX = (playerY-ray.rayY) * aTan+playerX;
		yOffset = -64;
		xOffset = -yOffset * aTan;
	}
	if(ray.rayAngle<PI)//Looking down
	{
		ray.rayY = (((int)playerY>>6)<<6)+64;
		ray.rayX = (playerY - ray.rayY) * aTan+playerX;
		yOffset = 64;
		xOffset = -yOffset * aTan;
	}
	if(ray.rayAngle == 0 || ray.rayAngle == PI)
	{
		ray.rayX = playerX;
		ray.rayY = playerY;
		depthOfField = 8;
	}
	while(depthOfField<8)
	{
		mapX = (int)(ray.rayX)>>6;
		mapY = (int)(ray.rayY)>>6;
		mapPosition = mapY * map.mapXLimit + mapX;
		if(mapPosition < map.mapXLimit*map.mapYLimit && map.mapArray[mapPosition]==1)//Hit Wall
		{
		       depthOfField = 8;
		}
		else //No Wall, Checks the next appearing horizontal line
		{
			ray.rayX += xOffset;
			ray.rayY += yOffset;
			depthOfField++;
		}
	}

		glColor3f(0,1,0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(playerX,playerY);
		glVertex2i(ray.rayX,ray.rayY);
		glEnd();
}


void drawRays()
{
	
	ray.rayAngle = playerAngle;
	for(int theray = 0; theray < NUMOFRAYS; theray++)
	{
		horizontalLineCheck();

	}
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	map.draw2DMap();
	drawPlayer();
	drawRays();
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

	playerDeltaX=cos(playerAngle)*5;
	playerDeltaY=sin(playerAngle)*5;

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

	playerDeltaX=cos(playerAngle)*5;
	playerDeltaY=sin(playerAngle)*5;

	ray.rayAngle = playerAngle;
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
