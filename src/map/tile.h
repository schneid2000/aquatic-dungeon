#pragma once

#include <string>

class Tile {
private:
	//Whether or not entities or the player can walk on the tile
	bool passable;

	//Whether or not the player can see the tile
	bool visible;

	//The type of tile (this determines the graphic and other values)
	std::string type;


public:
	//Default constructor
	Tile();
	//Recommended constructor
	Tile(bool passability, std::string new_type);

	//Getter for the passability
	bool get_passability();

	//Setter for the passability
	void set_passability(bool new_passability);

	//Getter for the type
	std::string get_type();

	
};