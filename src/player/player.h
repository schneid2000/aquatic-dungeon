#pragma once

#include "../map/coordinate.h"

class Player {
private:
	Coordinate current_tile;


public:
	//Empty constructor
	Player();

	//Initial constructor
	Player(Coordinate start_tile);

	//Getter for the current tile
	Coordinate get_current_tile();

	//Setter for the current tile
	void set_current_tile(int new_x, int new_y);

	int get_player_x();
	int get_player_y();

};