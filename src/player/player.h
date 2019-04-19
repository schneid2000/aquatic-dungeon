#pragma once

#include "../map/coordinate.h"
#include <string>

class Player {
private:
	Coordinate current_tile;
	std::string turn_direction;

	int health;
	int total_health;

public:
	//Empty constructor
	Player();

	//Initial constructor
	Player(Coordinate start_tile, int start_health);

	//Getter for the current tile
	Coordinate get_current_tile();

	//Setter for the current tile
	void set_current_tile(int new_x, int new_y);

	int get_player_x();
	int get_player_y();

	int get_health();
	int get_total_health();

	void change_health(int value);

	//Getter and setter for player turn direction
	void set_turn_direction(std::string new_direction);
	std::string get_turn_direction();

};