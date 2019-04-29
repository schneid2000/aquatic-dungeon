#include "player.h"

Player::Player() {
	for (int slot = 0; slot < kInventorySize; slot++) {
		occupied_slots[slot] = false;
	}
	selected_slot = -1;
}

Player::Player(Coordinate start_tile, int start_health, int start_strength) {
	current_tile = start_tile;
	total_health = start_health;
	health = start_health;
	strength = start_strength;
	for (int slot = 0; slot < kInventorySize; slot++) {
		occupied_slots[slot] = false;
	}
	selected_slot = -1;
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

int Player::get_health() {
	return health;
}

int Player::get_total_health() {
	return total_health;
}

int Player::get_strength() {
	return strength;
}

void Player::change_health(int value) {
	health += value;
}

//Gets the direction the player turned in
std::string Player::get_turn_direction() {
	return turn_direction;
}

//Sets the direction the player turned in
void Player::set_turn_direction(std::string new_direction) {
	turn_direction = new_direction;
}

//Returns true if the player's inventory is full
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


void Player::occupy_slot(int index) {
	occupied_slots[index] = true;
}

void Player::free_slot(int index) {
	occupied_slots[index] = false;
}

int Player::get_selected_slot() {
	return selected_slot;
}

void Player::set_selected_slot(int slot) {
	selected_slot = slot;
}