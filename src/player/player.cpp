#include "player.h"

//Empty constructor
Player::Player() {
	dead = false;
	for (int slot = 0; slot < kInventorySize; slot++) {
		occupied_slots[slot] = false;
	}
	selected_slot = -1;
}

//Recommended constructor
//Takes in the spawn tile of the player, their total health (which is also their initial health), and their initial strength
Player::Player(Coordinate start_tile, int start_health, int start_strength) {
	//Set the stats as passed
	current_tile = start_tile;
	total_health = start_health;
	health = start_health;
	strength = start_strength;

	//Intialize the inventory slots to empty
	for (int slot = 0; slot < kInventorySize; slot++) {
		occupied_slots[slot] = false;
	}

	//Initialize the selected slot to nothing
	selected_slot = -1;

	//The player is not initially dead
	dead = false;
}

//Getter for the current tile
Coordinate Player::get_current_tile() {
	return current_tile;
}

//Setter for the current tile
void Player::set_current_tile(int new_x, int new_y) {
	current_tile.set_coordinate(new_x, new_y);
}

//Get the player's x point
int Player::get_player_x() {
	return current_tile.get_coordinate_x();
}

//Get the player's y point
int Player::get_player_y() {
	return current_tile.get_coordinate_y();
}

//Get the player's current health
int Player::get_health() {
	return health;
}

//Get the player's total health
int Player::get_total_health() {
	return total_health;
}

//Get the player's strength (the default damage they do in an attack)
int Player::get_strength() {
	return strength;
}

//Adjust the player's health by the given value
//Positive restores health and negative does damage
void Player::change_health(int value) {
	//If the adjustment exceeds the player's total health, it will stay at the player's total health
	if (health + value > total_health) {
		health = total_health;
	}
	else {
		health += value;
	}

	//If the player has no health then they are dead
	if (get_health() <= 0) {
		dead = true;
	}
}

//Gets the direction the player turned in
std::string Player::get_turn_direction() {
	return turn_direction;
}

//Sets the direction the player turned in
void Player::set_turn_direction(std::string new_direction) {
	turn_direction = new_direction;
}

//Returns true if the player's 6-item inventory is full
bool Player::is_inventory_full() {
	for (int slot = kFirstItemIndex; slot < kInventorySize; slot++) {
		if (!occupied_slots[slot]) {
			return false;
		}
	}

	return true;
}


//Checks if a particular slot in the player's inventory is empty
bool Player::slot_is_empty(int index) {
	if (occupied_slots[index]) {
		return false;
	}

	return true;
}


//Gets the first empty slot in the 6 free slots of the player inventory
//Returns -1 if the player's inventory is full
int Player::get_first_empty_slot() {
	for (int slot = kFirstItemIndex; slot < kInventorySize; slot++) {
		if (!occupied_slots[slot]) {
			return slot;
		}
	}

	return -1;
}

//Gets the cooordinate of the first empty slot of the player's 6-slot inventory
Coordinate Player::coord_of_first_empty_slot() {
	int slot = get_first_empty_slot();
	int y = 4;
	int x = 2;
	if (slot > 5) {
		y += 1;
	}

	x += slot % 3;

	return Coordinate(x, y);
}

//Marks a slot of the player's inventory as filled
void Player::occupy_slot(int index) {
	occupied_slots[index] = true;
}

//Marks a slot of the player's inventory as empty
void Player::free_slot(int index) {
	occupied_slots[index] = false;
}

//Gets the value of the selected slot (-1 unselected, 0 - 8 if selected)
int Player::get_selected_slot() {
	return selected_slot;
}

//Set the selected slot value of the player
void Player::set_selected_slot(int slot) {
	selected_slot = slot;
}