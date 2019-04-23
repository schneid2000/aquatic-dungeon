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

void Level::add_random_room_randomly() {
	if (room_presets.size() == 0) {
		return;
	}
	int index = rand() % (room_presets.size());
	Coordinate random_tile = get_random_tile();
	Room random_room = room_presets[index];
	int room_tile_index = 0;
	for (int y = random_tile.get_coordinate_y(); y < random_tile.get_coordinate_y() + random_room.get_height(); y++) {
		for (int x = random_tile.get_coordinate_x(); x < random_tile.get_coordinate_x() + random_room.get_width(); x++) {
			if (is_valid_coordinate(x, y) && room_tile_index < random_room.size() && get_tile(x, y).get_type() != "floor") {
				map[y][x] = random_room.get_tile(room_tile_index);
			}
			room_tile_index++;
		}
	}
}

void Level::add_boss_room() {
	if (boss_presets.size() == 0) {
		return;
	}

	int index = rand() % (boss_presets.size());
	Coordinate random_tile = get_random_tile();
	Room boss_room = boss_presets[index];
	int tile_x = random_tile.get_coordinate_x();
	int tile_y = random_tile.get_coordinate_y();
	while (!is_valid_coordinate(tile_x + boss_room.get_width(), tile_y + boss_room.get_height())) {
		random_tile = get_random_tile();
		tile_x = random_tile.get_coordinate_x();
		tile_y = random_tile.get_coordinate_y();
	}
	
	int room_tile_index = 0;
	for (int y = random_tile.get_coordinate_y(); y < random_tile.get_coordinate_y() + boss_room.get_height(); y++) {
		for (int x = random_tile.get_coordinate_x(); x < random_tile.get_coordinate_x() + boss_room.get_width(); x++) {
			map[y][x] = boss_room.get_tile(room_tile_index);
			room_tile_index++;
		}
	}
}






void Level::generate_random_hall() {
	Coordinate current = get_random_impassable_tile();
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

	while (is_valid_coordinate(current_tile) && get_tile(x, y).get_type() != "floor") {
		map[y][x] = Tile(true, "floor");
		if (is_valid_coordinate(x + x_wall_modifier, y + y_wall_modifier) && get_tile(x + x_wall_modifier, y + y_wall_modifier).get_type() != "floor") {
			map[y + y_wall_modifier][x + x_wall_modifier] = Tile(false, "wall");
		}
		if (is_valid_coordinate(x - x_wall_modifier, y - y_wall_modifier) && get_tile(x - x_wall_modifier, y - y_wall_modifier).get_type() != "floor") {
			map[y - y_wall_modifier][x - x_wall_modifier] = Tile(false, "wall");
		}
		current_tile.set_coordinate(x + x_path_modifier, y + y_path_modifier);
		x = current_tile.get_coordinate_x();
		y = current_tile.get_coordinate_y();
	}

}

//Returns the percent of passable tiles in the level as a float from 0 to 1
float Level::percent_passable_tiles() {
	int floor_tiles = 0;

	for (int y = 0; y < kSize; y++) {
		for (int x = 0; x < kSize; x++) {
			if (map[y][x].get_type() == "floor") {
				floor_tiles++;
			}
		}
	}


	return (float) (((float) floor_tiles) / (float) (kSize * kSize));
}

//Generate the level by adding rooms, hallways, and the boss room
void Level::instantiate_level() {
	clear();
	while (percent_passable_tiles() < kPercentPassable) {
		add_random_room_randomly();
		add_random_room_randomly();
		generate_random_hall();
	}
	add_boss_room();
	setup_start_tiles();
	if (!path_to_gateway()) {
		std::cout << path_tiles.size() << std::endl;
		instantiate_level();
	}
	std::cout << path_tiles.size() << std::endl;
}


//Checks if there is a path from the spawn to the boss room
bool Level::path_to_gateway() {
	path_tiles.clear();
	tile_counter = 0;

	return check_for_path(start_tile);
}

bool Level::check_for_path(Coordinate search_center) {
	
	tile_counter++;
	path_tiles.push_back(search_center);
	if (is_valid_coordinate(search_center) && get_tile(search_center).get_type() == "boss_gateway") {
		std::cout << "goal reached" << std::endl;
		return true;
	}
	if (tile_counter > 900) {
		return false;
	}

	if (is_valid_coordinate(search_center.get_coordinate_x() + 1, search_center.get_coordinate_y()) 
		&& !seen_tile(Coordinate(search_center.get_coordinate_x() + 1, search_center.get_coordinate_y())) 
		&& (get_tile(search_center.get_coordinate_x() + 1, search_center.get_coordinate_y()).get_type() == "floor" || get_tile(search_center.get_coordinate_x() + 1, search_center.get_coordinate_y()).get_type() == "boss_gateway")) {
		return check_for_path(Coordinate(search_center.get_coordinate_x() + 1, search_center.get_coordinate_y()));
	}

	if (is_valid_coordinate(search_center.get_coordinate_x() - 1, search_center.get_coordinate_y()) 
		&& !seen_tile(Coordinate(search_center.get_coordinate_x() - 1, search_center.get_coordinate_y())) 
		&& (get_tile(search_center.get_coordinate_x() - 1, search_center.get_coordinate_y()).get_type() == "floor" || get_tile(search_center.get_coordinate_x() - 1, search_center.get_coordinate_y()).get_type() == "boss_gateway")) {
		return check_for_path(Coordinate(search_center.get_coordinate_x() - 1, search_center.get_coordinate_y()));
	}

	if (is_valid_coordinate(search_center.get_coordinate_x(), search_center.get_coordinate_y() + 1) 
		&& !seen_tile(Coordinate(search_center.get_coordinate_x(), search_center.get_coordinate_y() + 1)) 
		&& (get_tile(search_center.get_coordinate_x(), search_center.get_coordinate_y() + 1).get_type() == "floor" || get_tile(search_center.get_coordinate_x(), search_center.get_coordinate_y() + 1).get_type() == "boss_gateway")) {
		return check_for_path(Coordinate(search_center.get_coordinate_x(), search_center.get_coordinate_y() + 1));
	}

	if (is_valid_coordinate(search_center.get_coordinate_x(), search_center.get_coordinate_y() - 1) 
		&& !seen_tile(Coordinate(search_center.get_coordinate_x(), search_center.get_coordinate_y() - 1)) 
		&& (get_tile(search_center.get_coordinate_x(), search_center.get_coordinate_y() - 1).get_type() == "floor" || get_tile(search_center.get_coordinate_x(), search_center.get_coordinate_y() - 1).get_type() == "boss_gateway")) {
		return check_for_path(Coordinate(search_center.get_coordinate_x(), search_center.get_coordinate_y() - 1));
	}
	
	return false;
}

bool Level::seen_tile(Coordinate coordinate) {
	for (Coordinate tile : path_tiles) {
		if (coordinate.get_coordinate_x() == tile.get_coordinate_x() 
			&& coordinate.get_coordinate_y() == tile.get_coordinate_y()) {
			return true;
		}
	}

	return false;
}


void Level::clear() {
	for (int y = 0; y < kSize; y++) {
		for (int x = 0; x < kSize; x++) {
			map[y][x] = Tile(false, "ceiling");
		}
	}
}


