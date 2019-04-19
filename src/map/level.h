#pragma once

#include "tile.h"
#include "room.h"
#include "coordinate.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

//This represents the side lengths of the level array
//(the amount of space the level is allowed to generate in)
static const int kSize = 30;
static const float kPercentPassable = 0.5f;
static const std::string kPresetsLocation = "../bin/data/graphics/Presets/rooms.txt";

class Level {
private:
	//The map of tiles in the level
	Tile map[kSize][kSize];
	Coordinate start_tile;
	Coordinate goal_tile;
	Coordinate start_display_tile;
	std::vector<Room> room_presets;
	std::vector<Room> boss_presets;

	


public:
	//The default constructor
	Level();

	//Dev constructor
	Level(bool passable);

	//Getter for specific tile
	Tile get_tile(int x, int y);
	Tile get_tile(Coordinate coordinate);

	//Setter for specific tile
	void set_tile(int x, int y, Tile tile);

	//Returns a number of all the passable tiles
	int count_all_passable_tiles();
	
	//Returns a coordinate of a random passable tile
	Coordinate get_random_passable_tile();

	//Returns a coordinate of a random impassable tile
	Coordinate get_random_impassable_tile();

	//Returns a coordinate of any random tile
	Coordinate get_random_tile();

	//Sets the start tile and the start display tile
	void setup_start_tiles();

	//Getter for the player's spawn point
	Coordinate get_start_tile();

	//Getter for the display window's start point
	Coordinate get_start_display_tile();

	bool is_valid_coordinate(int x, int y);
	bool is_valid_coordinate(Coordinate coordinate);

	//Give the level the room presets
	void load_room_presets();

	//adds a room to the level
	void add_random_room_randomly();

	//Makes a random hallway
	void generate_random_hall();

	//Places a boss room in the level
	void add_boss_room();


	//Creates a hall in a specified direction
	void create_hall_in_direction(Coordinate current_tile, bool up_or_right, bool opposite);

	float percent_passable_tiles();

	void instantiate_level();

	bool path_to_gateway();
};