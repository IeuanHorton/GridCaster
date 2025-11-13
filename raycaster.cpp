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
#include "raycaster.h"

#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DEGREERADIAN 0.0174533 //1 degree in radians

Map map;
Player player;

int NUMOFRAYS = 60;
int RAYOFFSET = NUMOFRAYS/2;

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

float distanceOfRay(float ax, float ay, float bx, float by, float angle)
{
	return (sqrt((bx-ax)*(bx-ax) + (by-ay) * (by-ay)));//Pythagorean Theorem
}

float depthOfFieldCheck(int depthOfField, float xOffset, float yOffset, Ray* ray)
{
	int mapX, mapY, mapPosition;

	while(depthOfField<8)
	{
		mapX = (int)(ray->rayX)>>6;
		mapY = (int)(ray->rayY)>>6;
		mapPosition = mapY * map.mapXLimit + mapX;
		if(mapPosition>0 && mapPosition < map.mapXLimit*map.mapYLimit && map.mapArray[mapPosition]==1)//Hit Wall
		{
			return distanceOfRay(player.X, player.Y, ray->rayX, ray->rayY, ray->rayAngle);			
		}
		else //No Wall, Checks the next appearing wall
		{
			ray->rayX += xOffset;
			ray->rayY += yOffset;
			depthOfField++;
		}
	}
	return 900000;
}
void horizontalLineCheck(Ray* ray)
{
	int depthOfField;
	float yOffset, xOffset, aTan;

	depthOfField = 0;
   aTan = -1/tan(ray->rayAngle);//THIS IS BAD. WILL THROW A SEGFAULT IF RAYANGLE IS 0. EASIER TO LEAVE IT AS IT DOESN'T SEEM TO HAPPEN IF YOU DON'T INIT THE RAYANGLE AT 0.
	if(LookingUp(ray))
	{
		ray->rayY = (((int)player.Y>>6)<<6)-0.0001;
		ray->rayX = (player.Y-ray->rayY) * aTan+player.X;
		yOffset = -64;
		xOffset = -yOffset * aTan;
	}
	if(LookingDown(ray))
	{
		ray->rayY = (((int)player.Y>>6)<<6)+64;
		ray->rayX = (player.Y - ray->rayY) * aTan+player.X;
		yOffset = 64;
		xOffset = -yOffset * aTan;
	}
	if(ray->rayAngle == 0 || ray->rayAngle == PI)
	{
		ray->rayX = player.X;
		ray->rayY = player.Y;
		depthOfField = 8;
	}
	ray->horizontalDistance = depthOfFieldCheck(depthOfField, xOffset, yOffset, ray);
	ray->horizontalX = ray->rayX;
	ray->horizontalY = ray->rayY;
}

void verticalLineCheck(Ray* ray)
{
	int depthOfField;
	float yOffset, xOffset, nTan;

	depthOfField = 0;
   nTan = -tan(ray->rayAngle);
	if(LookingLeft(ray))
	{
		ray->rayX = (((int)player.X>>6)<<6)-0.0001;
		ray->rayY = (player.X-ray->rayX) * nTan+player.Y;
		xOffset = -64;
		yOffset = -xOffset * nTan;
	}
	if(LookingRight(ray))
	{
		ray->rayX = (((int)player.X>>6)<<6)+64;
		ray->rayY = (player.X - ray->rayX) * nTan+player.Y;
		xOffset = 64;
		yOffset = -xOffset * nTan;
	}
	if(ray->rayAngle == 0 || ray->rayAngle == PI)
	{
		ray->rayX = player.X;
		ray->rayY = player.Y;
		depthOfField = 8;
	}
	ray->verticalDistance = depthOfFieldCheck(depthOfField, xOffset, yOffset, ray);
	ray->verticalX = ray->rayX;
	ray->verticalY = ray->rayY;
}


void drawRays()
{
	Ray ray;	
	ray.rayAngle = player.angle - DEGREERADIAN*RAYOFFSET;

	for(int theray = 0; theray < NUMOFRAYS; theray++)
	{
		horizontalLineCheck(&ray);
		verticalLineCheck(&ray);

		glColor3f(1,0,0);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2i(player.X,player.Y);
		if(ray.horizontalDistance <= ray.verticalDistance)
		{
			ray.rayX = ray.horizontalX;
			ray.rayY = ray.horizontalY;
		}
		else
		{
			ray.rayX = ray.verticalX;
			ray.rayY = ray.verticalY;
		}

		glVertex2i(ray.rayX,ray.rayY);
		glEnd();
		ray.rayAngle+=DEGREERADIAN;
		if(ray.rayAngle < 0)
		{
			ray.rayAngle += 2*PI;
		}
		if(ray.rayAngle > 2*PI)
		{
			ray.rayAngle -=2*PI;
		}
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


	glutPostRedisplay();
}

void init()
{
	glClearColor(0.3,0.3,0.3,0);
	gluOrtho2D(0,WINDOWWIDTH,WINDOWHEIGHT,0);

   player.angle = PI + (PI/4);//Throws segfault if not done

	player.deltaX=cos(player.angle)*5;
	player.deltaY=sin(player.angle)*5;

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

bool LookingUp(Ray* ray){
	return ray->rayAngle>PI;
}
bool LookingDown(Ray* ray){
	return ray->rayAngle<PI;
}
bool LookingRight(Ray* ray){
	return (ray->rayAngle<P2 || ray->rayAngle>P3);
}
bool LookingLeft(Ray* ray){
	return (ray->rayAngle>P2 && ray->rayAngle<P3);
}
