#pragma once

#include "../map/coordinate.h"
#include <string>

static const int kInventorySize = 9;
static const int kFirstItemIndex = 3;

class Player {
private:
	//The player's current position
	Coordinate current_tile;

	//The direction the player is facing
	std::string turn_direction;

	//The player's current health
	int health;

	//The player's total health
	int total_health;

	//The base damage the player does to enemies
	int strength;

	//Represents whether slots in the player's inventory are filled
	//Filled slots are true, empty slots are false
	bool occupied_slots[kInventorySize];

	//The slot a player has selected (-1 if no slot is selected)
	int selected_slot;

	//True if the player is dead
	bool dead;

public:
	//Empty constructor
	Player();

	//Initial constructor
	Player(Coordinate start_tile, int start_health, int start_strength);

	//Getter for the current tile
	Coordinate get_current_tile();

	//Setter for the current tile
	void set_current_tile(int new_x, int new_y);

	//Get the player's x coordinate
	int get_player_x();

	//Get the player's y coordinate
	int get_player_y();

	//Get the player's current health
	int get_health();

	//Get the player's total health
	int get_total_health();

	//Get the player's base damage
	int get_strength();


	//Adjust the player's current health by the given value
	//Positive represents healing, negative represents damage
	//If positive the player's health will not exceed their total health
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

	//Changes a slot in the player's inventory to full
	void occupy_slot(int index);

	//Changes a slot in the player's inventory to empty
	void free_slot(int index);

	//Gets the relative coordinate of the first empty slot (non-special) in the player's inventory
	Coordinate coord_of_first_empty_slot();

	//Get the player's selected slot 
	int get_selected_slot();

	//Set the player's selected slot
	void set_selected_slot(int slot);
};