#include "player.h"

Player::Player() {

}

Player::Player(Coordinate start_tile, int start_health, int start_strength) {
	current_tile = start_tile;
	total_health = start_health;
	health = start_health;
	strength = start_strength;
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