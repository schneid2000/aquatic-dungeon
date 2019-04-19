#pragma once

#include "tile.h"
#include <vector>
#include <string>
#include <iostream>

class Room {
private:
	//This hold the tiles in the room
	std::vector<Tile> room_tiles;

	//The height and width (for use in generation of the room)
	int height;
	int width;
	bool boss;

public:
	//Empty constructor
	Room();

	//Constructor taking a height, width, and a string representing the characters of the room
	Room(int new_height, int new_width, std::string tiles);

	//Getter for the height and width
	int get_height();
	int get_width();
	bool has_boss();

	//Setter for the boss value
	void set_boss(bool value);

	//Gets the total area of the room
	int size();

	//Get a particular tile from the room by index
	Tile get_tile(int index);

	//Overloading input and output streams to input rooms from a file
	friend std::istream& operator>>(std::istream &input, Room &room);
	friend std::ostream& operator<<(std::ostream &output, const Room &room);
};