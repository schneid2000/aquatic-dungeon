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
	std::vector<Room> room_presets;
	std::vector<Room> boss_presets;
	std::vector<Coordinate> path_tiles;
	int tile_counter;

	


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

	//Returns true if the coordinate can be used (is in the map)
	bool is_valid_coordinate(int x, int y);
	bool is_valid_coordinate(Coordinate coordinate);

	//Returns true if the tile at the coordinate is valid and passable
	bool is_valid_passable_tile(int x, int y);
	bool is_valid_passable_tile(Coordinate coordinate);

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

	//Get the percent of passable tiles out of the level's dimensions
	float percent_passable_tiles();

	//Fill the level with rooms, hallways, and a boss room
	void instantiate_level();

	//Find a path from the spawn to the boss room
	bool path_to_gateway();
	bool check_for_path(Coordinate search_center);

	//Returns true if a tile is in the path_tiles vector
	bool seen_tile(Coordinate coordinate);

	//Clears out the level (used for repeated instantiation)
	void clear();

	//Gets a random passable tile in a certain (square, NOT circular) radius from the center coordinate
	//If there is no such tile, it will return the search center as a coordinate
	Coordinate get_random_passable_tile_in_radius(int x_center, int y_center, int radius);
	Coordinate get_random_passable_tile_in_radius(Coordinate center, int radius);
};