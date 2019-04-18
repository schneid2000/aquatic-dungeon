#pragma once

#include "tile.h"
#include "coordinate.h"
#include <vector>
#include <cstdlib>
#include <ctime>

//This represents the side lengths of the level array
//(the amount of space the level is allowed to generate in)
static const int kSize = 30;

class Level {
private:
	//The map of tiles in the level
	Tile map[20][20];
	Coordinate start_tile;
	Coordinate goal_tile;
	Coordinate start_display_tile;

	


public:
	//The default constructor
	Level();

	//Getter for specific tile
	Tile get_tile(int y, int x);

	//Setter for specific tile
	void set_tile(int y, int x, Tile tile);

	//Returns a number of all the passable tiles
	int count_all_passable_tiles();
	
	//Returns a pointer to a randomly determined passable tile
	Coordinate get_random_passable_tile();

	//Sets the start tile and the display start tile
	void setup_start_tiles();

};