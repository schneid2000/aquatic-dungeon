#include "level.h"

//Empty constructor
Level::Level() {

}

//Constructor
Level::Level(bool passable) {
	//If passed with true, generate a completely passable floor
	if (passable) {
		for (int y = 0; y < kSize; y++) {
			for (int x = 0; x < kSize; x++) {
				map[y][x] = Tile(true, "floor");
			}
		}
	}
	//If passed with false, generate a completely impassable floor
	else {
		for (int y = 0; y < kSize; y++) {
			for (int x = 0; x < kSize; x++) {
				map[y][x] = Tile(false, "ceiling");
			}
		}
	}
}

//Gets a specific map tile by the x and y coordinate
Tile Level::get_tile(int x, int y) {
	if (is_valid_coordinate(x, y)) {
		return map[y][x];
	}
	
}

//Gets a specific map tile by the coordinate
Tile Level::get_tile(Coordinate coordinate) {
	return get_tile(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}

//Sets a specific map tile by the coordinate
void Level::set_tile(int x, int y, Tile tile) {
	map[y][x] = tile;
}

//Returns a count of all the passable tiles in the level
int Level::count_all_passable_tiles() {
	int count = 0;

	//Go through all the tiles
	//If a tile is passable add it to the count
	for (int y = 0; y < kSize; y++) {
		for (int x = 0; x < kSize; x++) {
			if (map[y][x].get_passability()) {
				count++;
			}
		}
	}

	return count;
}

//Help with random number generation http://www.cplusplus.com/reference/cstdlib/rand/
Coordinate Level::get_random_passable_tile() {
	//Get a random y and x index (within the level)
	int y = rand() % kSize;
	int x = rand() % kSize;

	//If that random tile is not passable, continue searching
	while (!is_valid_coordinate(x, y) || !map[y][x].get_passability() || map[y][x].get_type() != "floor") {
		y = rand() % kSize;
		x = rand() % kSize;
	}

	//Return the coordinate representing that tile
	return Coordinate(x, y);
}

//Get a random ceiling tile
Coordinate Level::get_random_impassable_tile() {
	//Get a random x and y index (within the level)
	int y = rand() % kSize;
	int x = rand() % kSize;

	//Continues searching if we have not reached a ceiling tile
	while (!is_valid_coordinate(x, y) || map[y][x].get_type() != "ceiling") {
		y = rand() % kSize;
		x = rand() % kSize;
	}

	return Coordinate(x, y);
}

//Get a random tile
Coordinate Level::get_random_tile() {
	//Get a random x and y index (within the level)
	int y = rand() % kSize;
	int x = rand() % kSize;
	while (!is_valid_coordinate(x, y)) {
		int y = rand() % kSize;
		int x = rand() % kSize;
	}

	return Coordinate(x, y);
}


//Set the player's start tile
void Level::setup_start_tiles() {
	//The start tile should be any random passable tile
	start_tile = get_random_passable_tile();

}

//Get the player start tile
Coordinate Level::get_start_tile() {
	return start_tile;
}


//Checks if the coordinate is valid on the map
bool Level::is_valid_coordinate(int x, int y) {
	//If the x and y indices are inside of map bounds, they are valid
	if (0 <= x && x < kSize && 0 <= y && y < kSize) {
		return true;
	}

	return false;
}

//Checks if the Coordinate is valid on the map
bool Level::is_valid_coordinate(Coordinate coordinate) {
	return (is_valid_coordinate(coordinate.get_coordinate_x(), coordinate.get_coordinate_y()));
}

bool Level::is_valid_passable_tile(int x, int y) {
	return (is_valid_coordinate(x, y) && map[y][x].get_passability());
}

bool Level::is_valid_passable_tile(Coordinate coordinate) {
	return is_valid_passable_tile(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}


//Loads the room presets located in rooms.txt in the Presets folder
void Level::load_room_presets() {
	std::fstream filestream;
	filestream.open(kPresetsLocation, std::fstream::in);
	Room current_preset;
	while (filestream >> current_preset) {
		std::cout << "Loading rooms..." << std::endl;
		if (current_preset.has_boss()) {
			boss_presets.push_back(current_preset);
		} else {
			room_presets.push_back(current_preset);
		}
	}
}

//Chooses a random room from the room presets and adds it to the level
void Level::add_random_room_randomly() {
	if (room_presets.size() == 0) {
		return;
	}

	//Selects a random room from the preset vector (should be loaded beforehand)
	int index = rand() % (room_presets.size());

	//Gets a random tile to start as the corner of the room
	Coordinate random_tile = get_random_tile();
	Room random_room = room_presets[index];
	int room_tile_index = 0;

	//Goes through and adds the tiles of the rooms as per the room object states. 
	//Parts of the room that go off the map are not generated and additionally rooms can partially overlap.
	for (int y = random_tile.get_coordinate_y(); y < random_tile.get_coordinate_y() + random_room.get_height(); y++) {
		for (int x = random_tile.get_coordinate_x(); x < random_tile.get_coordinate_x() + random_room.get_width(); x++) {
			//The room will not try to override already existing floor tiles.
			if (is_valid_coordinate(x, y) && room_tile_index < random_room.size() && get_tile(x, y).get_type() != "floor") {
				map[y][x] = random_room.get_tile(room_tile_index);
			}
			room_tile_index++;
		}
	}
}


//Adds the boss room into the level
void Level::add_boss_room() {
	if (boss_presets.size() == 0) {
		return;
	}

	//Choose a random boss room from the boss room presets (loaded beforehand)
	int index = rand() % (boss_presets.size());

	//Gets a random tile to act as the corner for generating the boss room
	Coordinate random_tile = get_random_tile();
	Room boss_room = boss_presets[index];
	int tile_x = random_tile.get_coordinate_x();
	int tile_y = random_tile.get_coordinate_y();

	//Rerolls the tile until the entire boss room is guaranteed to be generated
	while (!is_valid_coordinate(tile_x + boss_room.get_width(), tile_y + boss_room.get_height())) {
		random_tile = get_random_tile();
		tile_x = random_tile.get_coordinate_x();
		tile_y = random_tile.get_coordinate_y();
	}
	
	int room_tile_index = 0;

	//Puts the boss room in the level. The boss room will override all tiles already underneath it.
	for (int y = random_tile.get_coordinate_y(); y < random_tile.get_coordinate_y() + boss_room.get_height(); y++) {
		for (int x = random_tile.get_coordinate_x(); x < random_tile.get_coordinate_x() + boss_room.get_width(); x++) {
			map[y][x] = boss_room.get_tile(room_tile_index);
			room_tile_index++;
		}
	}
}


//Generates a random hallway
void Level::generate_random_hall() {
	//The hallway starts from a non-passable tile
	Coordinate current = get_random_impassable_tile();

	//The direction is randomly selected
	int up_right_choice = rand() % 2;
	int opposite_choice = rand() % 2;
	create_hall_in_direction(current, up_right_choice, opposite_choice);
	
}

//up = true
//right = false
void Level::create_hall_in_direction(Coordinate current_tile, bool up_or_right, bool opposite) {
	int x_path_modifier = 1;
	int y_path_modifier = 1;
	int x_wall_modifier = 0;
	int y_wall_modifier = 0;
	if (up_or_right) {
		//Hall moves up
		//The path will not be moving horizontally
		x_path_modifier = 0;
		//We will be filling in walls to the left and right of the hall
		x_wall_modifier = 1;

	}
	else {
		//Hall moves right
		//The path will not be moving vertically
		y_path_modifier = 0;
		//We will be filling in walls to the top and bottom of the hall
		y_wall_modifier = 1;
	}

	if (opposite) {
		//Changes the path from right to left or up to down
		x_path_modifier *= -1;
		y_path_modifier *= -1;
	}

	int x = current_tile.get_coordinate_x();
	int y = current_tile.get_coordinate_y();

	//The hall will continue to generate until it hits an existing floor tile or the edge of the level
	while (is_valid_coordinate(current_tile) && get_tile(x, y).get_type() != "floor") {
		//Adds a floor tile and then a wall on both sides, based on the direction
		map[y][x] = Tile(true, "floor");
		if (is_valid_coordinate(x + x_wall_modifier, y + y_wall_modifier) && get_tile(x + x_wall_modifier, y + y_wall_modifier).get_type() != "floor") {
			map[y + y_wall_modifier][x + x_wall_modifier] = Tile(false, "wall");
		}
		if (is_valid_coordinate(x - x_wall_modifier, y - y_wall_modifier) && get_tile(x - x_wall_modifier, y - y_wall_modifier).get_type() != "floor") {
			map[y - y_wall_modifier][x - x_wall_modifier] = Tile(false, "wall");
		}

		//Update the variables used in the loop
		current_tile.set_coordinate(x + x_path_modifier, y + y_path_modifier);
		x = current_tile.get_coordinate_x();
		y = current_tile.get_coordinate_y();
	}

}

//Returns the percent of passable tiles in the level as a float from 0 to 1
float Level::percent_passable_tiles() {
	int floor_tiles = 0;

	//Get the number of floor tiles
	for (int y = 0; y < kSize; y++) {
		for (int x = 0; x < kSize; x++) {
			if (map[y][x].get_type() == "floor") {
				floor_tiles++;
			}
		}
	}

	//Return the number of floor tiles over the total size of the level
	return (float) (((float) floor_tiles) / (float) (kSize * kSize));
}

//Generate the level by adding rooms, hallways, and the boss room
void Level::instantiate_level() {
	//In the case of a recursion, reset any rooms that were previously generated in the level
	clear();

	//Until the percent of floor tiles is sufficient, keep adding rooms and halls
	while (percent_passable_tiles() < kPercentPassable) {
		add_random_room_randomly();
		add_random_room_randomly();
		generate_random_hall();
	}

	//Once enough rooms have been added, add the boss room
	add_boss_room();

	//Set up the tile where the player will spawn
	setup_start_tiles();

	//Create a new level if a path from the spawn to the boss room cannot be found
	if (!path_to_gateway()) {
		instantiate_level();
	}
}


//Checks if there is a path from the spawn to the boss room
bool Level::path_to_gateway() {
	path_tiles.clear();
	tile_counter = 0;

	return check_for_path(start_tile);
}


//Checks for a path between the given tile and the boss room
bool Level::check_for_path(Coordinate search_center) {
	//Increase the counter each time the path is searched for
	tile_counter++;
	//Add the seen tile to the path_tiles vector
	path_tiles.push_back(search_center);

	//Return true if we have reached a boss gateway tile
	if (is_valid_coordinate(search_center) && get_tile(search_center).get_type() == "boss_gateway") {
		return true;
	}

	//If a search has occurred an obscene amount of times, return false
	if (tile_counter > 900) {
		return false;
	}

	int x = search_center.get_coordinate_x();
	int y = search_center.get_coordinate_y();

	//Check all of the cardinal directions, above, left, right, and below this tile
	if (can_be_path_tile(x + 1, y)) {
		return check_for_path(Coordinate(x + 1, y));
	}

	if (can_be_path_tile(x - 1, y)) {
		return check_for_path(Coordinate(x - 1, y));
	}

	if (can_be_path_tile(x, y + 1)) {
		return check_for_path(Coordinate(x, y + 1));
	}

	if (can_be_path_tile(x, y - 1)) {
		return check_for_path(Coordinate(x, y - 1));
	}
	
	//If none are true return false
	return false;
}

//Checks if a path can move to this tile
bool Level::can_be_path_tile(int x, int y) {
	//Return true if the coordinate is valid and the tile is not seen yet and the tile is a floor or boss gateway tile
	if (is_valid_coordinate(x, y) 
		&& !seen_tile(Coordinate(x, y)) 
		&& (get_tile(x, y).get_type() == "floor" || get_tile(x, y).get_type() == "boss_gateway")) {
		return true;
	}

	return false;
}

//Returns true if the tile has been seen
bool Level::seen_tile(Coordinate coordinate) {
	//Return true if the tile is in the path_tiles vector
	for (Coordinate tile : path_tiles) {
		if (coordinate == tile) {
			return true;
		}
	}

	return false;
}


//Resets the level so it can be generated again
void Level::clear() {
	for (int y = 0; y < kSize; y++) {
		for (int x = 0; x < kSize; x++) {
			map[y][x] = Tile(false, "ceiling");
		}
	}
}

//Gets a random tile in the sqaure (NOT circular) radius
Coordinate Level::get_random_passable_tile_in_radius(int x_center, int y_center, int radius) {
	std::vector<Coordinate> tiles;
	for (int y = y_center - radius; y <= y_center + radius; y++) {
		for (int x = x_center - radius; x <= x_center + radius; x++) {
			if (is_valid_passable_tile(x, y)) {
				tiles.push_back(Coordinate(x, y));
			}
		}
	}

	//Get a random tile from those found
	if (tiles.size() > 0) {
		int index = rand() % tiles.size();
		return tiles[index];
	} else {
		//If none were found return the center
		return Coordinate(x_center, y_center);
	}

}

//Gets a random tile in the square (NOT circular) radius based on the coordinate
Coordinate Level::get_random_passable_tile_in_radius(Coordinate center, int radius) {
	return get_random_passable_tile_in_radius(center.get_coordinate_x(), center.get_coordinate_y(), radius);
}