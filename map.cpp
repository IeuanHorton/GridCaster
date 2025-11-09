#include "map.h"
#include <GL/glut.h>

int Map::mapArray[]=
{
	1,1,1,1,1,1,1,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1
};





void Map::draw2DMap(){
	int xOffset = 0;
	int yOffset = 0;

	for(int y = 0; y < mapYLimit; y++)
	{
		for(int x = 0; x < mapXLimit; x++)
		{
			if(mapArray[y*mapXLimit+x]==1)
			{
				glColor3f(1,1,1);
			}
			else
			{
				glColor3f(0,0,0);
			}

			xOffset = x * mapPixelSize;
			yOffset = y * mapPixelSize;

			glBegin(GL_QUADS);
			glVertex2i(xOffset, yOffset);
			glVertex2i(xOffset, yOffset + mapPixelSize);
		        glVertex2i(xOffset + mapPixelSize, yOffset + mapPixelSize);
			glVertex2i(xOffset + mapPixelSize, yOffset);
			glEnd();	
		}
	}
}
