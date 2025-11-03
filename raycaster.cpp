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
#define P2 PI/2
#define P3 3*PI/2

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
	float yOffset, xOffset, aTan;

	depthOfField = 0;
   aTan = -1/tan(ray.rayAngle);//THIS IS BAD. WILL THROW A SEGFAULT IF RAYANGLE IS 0. EASIER TO LEAVE IT AS IT DOESN'T SEEM TO HAPPEN IF YOU DON'T INIT THE RAYANGLE AT 0.
	if(ray.rayAngle>PI)//Looking up
	{
		ray.rayY = (((int)player.Y>>6)<<6)-0.0001;
		ray.rayX = (player.Y-ray.rayY) * aTan+player.X;
		yOffset = -64;
		xOffset = -yOffset * aTan;
	}
	if(ray.rayAngle<PI)//Looking down
	{
		ray.rayY = (((int)player.Y>>6)<<6)+64;
		ray.rayX = (player.Y - ray.rayY) * aTan+player.X;
		yOffset = 64;
		xOffset = -yOffset * aTan;
	}
	if(ray.rayAngle == 0 || ray.rayAngle == PI)
	{
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
		glLineWidth(5);
		glBegin(GL_LINES);
		glVertex2i(player.X,player.Y);
		glVertex2i(ray.rayX,ray.rayY);
		glEnd();
}

void verticalLineCheck()
{
	int depthOfField, mapX, mapY, mapPosition;
	float yOffset, xOffset, nTan;

	depthOfField = 0;
   nTan = -tan(ray.rayAngle);
	if(ray.rayAngle>P2 && ray.rayAngle<P3)//Looking left
	{
		ray.rayX = (((int)player.X>>6)<<6)-0.0001;
		ray.rayY = (player.X-ray.rayX) * nTan+player.Y;
		xOffset = -64;
		yOffset = -xOffset * nTan;
	}
	if(ray.rayAngle<P2 || ray.rayAngle>P3)//Looking right
	{
		ray.rayX = (((int)player.X>>6)<<6)+64;
		ray.rayY = (player.X - ray.rayX) * nTan+player.Y;
		xOffset = 64;
		yOffset = -xOffset * nTan;
	}
	if(ray.rayAngle == 0 || ray.rayAngle == PI)
	{
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

		glColor3f(1,0,0);
		glLineWidth(2);
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
		verticalLineCheck();
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
	key = tolower(key);

	if(key == LEFT)
	{
		player.angle -= player.TURNSPEED;
		if(player.angle < 0)
		{
			player.angle+=(2*PI);
		}
		player.deltaX=cos(player.angle)*5;
		player.deltaY=sin(player.angle)*5;
	}
	
	if(key == RIGHT)
	{
		player.angle += player.TURNSPEED;
		if(player.angle > 2*PI)//Resets the angle is it goes over 360 or under 0
		{
			player.angle-=(2*PI);
		}
		player.deltaX=cos(player.angle)*5;
		player.deltaY=sin(player.angle)*5;
	}

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

	ray.rayAngle = player.angle;

	glutPostRedisplay();
}

void init()
{
	glClearColor(0.3,0.3,0.3,0);
	gluOrtho2D(0,WINDOWWIDTH,WINDOWHEIGHT,0);

   player.angle = PI + (PI/2);//Throws segfault if not done

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
