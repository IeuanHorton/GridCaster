#ifndef MAP_H
#define MAP_H

class Map
{

	public:

	int mapXLimit = 8;
	int mapYLimit = 8;
	int mapPixelSize = 64;

	void draw2DMap();

	static int mapArray[];
};
#endif
