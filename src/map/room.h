#pragma once

#include "tile.h"
#include <vector>
#include <string>
#include <iostream>

class Room {
private:
	std::vector<Tile> room_tiles;
	int height;
	int width;

public:
	Room();
	Room(int new_height, int new_width, std::string tiles);

	int get_height();
	int get_width();
	int size();

	Tile get_tile(int index);


	friend std::istream& operator>>(std::istream &input, Room &room);
	friend std::ostream& operator<<(std::ostream &output, const Room &room);
};