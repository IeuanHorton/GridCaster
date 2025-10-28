#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#include <iostream> 

#include "player.h"
#include "keybindings.h"
#include "settings.h"
#include "map.h"
#include "ray.h"

#define PI 3.1415926535

Map map;
Ray ray;
Player player;

int NUMOFRAYS = 1;

void drawPlayer()
{
	glColor3f(1,1,0); //Yellow
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(player.X,player.Y);
	glEnd();

	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(player.X,player.Y);
	glVertex2i(player.X+player.deltaX*5,player.Y+player.deltaY*5);
	glEnd();
}

void printText(std::string text)
{
	std::cout << text; 
}

void horizontalLineCheck()
{
	int depthOfField, mapX, mapY, mapPosition;
	float yOffset, xOffset;


	depthOfField = 0;
	float aTan = -1/tan(ray.rayAngle);
	if(ray.rayAngle>PI)//Looking up
	{
		printText("looking up");
		ray.rayY = (((int)player.Y>>6)<<6)-0.0001;
		ray.rayX = (player.Y-ray.rayY) * aTan+player.X;
		yOffset = -64;
		xOffset = -yOffset * aTan;
	}
	if(ray.rayAngle<PI)//Looking down
	{
		printText("looking down");
		ray.rayY = (((int)player.Y>>6)<<6)+64;
		ray.rayX = (player.Y - ray.rayY) * aTan+player.X;
		yOffset = 64;
		xOffset = -yOffset * aTan;
	}
	if(ray.rayAngle == 0 || ray.rayAngle == PI)
	{
		printText("looking flat");
		ray.rayX = player.X;
		ray.rayY = player.Y;
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
		glVertex2i(player.X,player.Y);
		glVertex2i(ray.rayX,ray.rayY);
		glEnd();
}


void drawRays()
{
	
	ray.rayAngle = player.angle;
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
		player.angle -= player.TURNSPEED;
	}
	
	if(key == RIGHT)
	{
		player.angle += player.TURNSPEED;
	}

	if(player.angle > 2*PI)//Resets the angle is it goes over 360 or under 0
	{
		player.angle-=(2*PI);
	}
	if(player.angle < 0)
	{
		player.angle+=(2*PI);
	}

	player.deltaX=cos(player.angle)*5;
	player.deltaY=sin(player.angle)*5;

	if(key == UP)
	{
		player.X += player.deltaX;
		player.Y += player.deltaY;
	}
	if(key == DOWN)
	{
		player.X -= player.deltaX;
		player.Y -= player.deltaY;
	}

	glutPostRedisplay();
}

void init()
{
	glClearColor(0.3,0.3,0.3,0);
	gluOrtho2D(0,WINDOWWIDTH,WINDOWHEIGHT,0);

	player.deltaX=cos(player.angle)*5;
	player.deltaY=sin(player.angle)*5;

	ray.rayAngle = player.angle;
	player.X=300; player.Y=300;
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
