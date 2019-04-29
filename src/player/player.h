#pragma once

#include "../map/coordinate.h"
#include <string>

static const int kInventorySize = 9;
static const int kFirstItemIndex = 3;

class Player {
private:
	Coordinate current_tile;
	std::string turn_direction;

	int health;
	int total_health;

	int strength;

	bool occupied_slots[kInventorySize];

public:
	//Empty constructor
	Player();

	//Initial constructor
	Player(Coordinate start_tile, int start_health, int start_strength);

	//Getter for the current tile
	Coordinate get_current_tile();

	//Setter for the current tile
	void set_current_tile(int new_x, int new_y);

	int get_player_x();
	int get_player_y();

	int get_health();
	int get_total_health();

	int get_strength();

	void change_health(int value);

	//Getter and setter for player turn direction
	void set_turn_direction(std::string new_direction);
	std::string get_turn_direction();


	//Returns true if the player's (6-item) inventory is full
	bool is_inventory_full();

	//Returns true if an inventory slot is empty
	bool slot_is_empty(int index);

	//Returns the index of the first empty slot in the (6-item) inventory
	int get_first_empty_slot();

	void occupy_slot(int index);
	void free_slot(int index);

	Coordinate coord_of_first_empty_slot();
};