#include "tile.h"

//The default constructor creates an impassable floor tile 
//This is simplify level generation, as passable rooms will be created in an impassable void
Tile::Tile() {
	type = "ceiling";
	passable = false;
}

//The recommended constructor takes in a passability and type 
Tile::Tile(bool passability, std::string new_type) {
	passable = passability;
	type = new_type;
}

//Getter for the passability
bool Tile::get_passability() {
	return passable;
}

//Setter for the passability
void Tile::set_passability(bool new_passability) {
	passable = new_passability;
}

//Getter for the type
std::string Tile::get_type() {
	return type;
}
