#include "level.h"

//The default level constructor will create a 20x20 map of normal floor tiles
Level::Level() {
	/*for (int y = 0; y < kSize; y++) {
		for (int x = 0; x < kSize; x++) {
			map[y][x].set_passability(false);
		}
	}*/
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

//Gets a specific map tile by the coordinate
Tile Level::get_tile(int y, int x) {
	return map[y][x];
}

Tile Level::get_tile(Coordinate coordinate) {
	return get_tile(coordinate.get_coordinate_y(), coordinate.get_coordinate_x());
}

//Sets a specific map tile by the coordinate
void Level::set_tile(int y, int x, Tile tile) {
	map[y][x] = tile;
}

int Level::count_all_passable_tiles() {
	int count = 0;

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
	int y = rand() % kSize;
	int x = rand() % kSize;

	while (!map[y][x].get_passability()) {
		y = rand() % kSize;
		x = rand() % kSize;
	}

	return Coordinate(x, y);
}

//Get a random ceiling tile
Coordinate Level::get_random_void_tile() {
	int y = rand() % kSize;
	int x = rand() % kSize;

	while (map[y][x].get_type() == "ceiling") {
		y = rand() % kSize;
		x = rand() % kSize;
	}

	return Coordinate(x, y);
}

Coordinate Level::get_random_tile() {
	int y = rand() % kSize;
	int x = rand() % kSize;

	return Coordinate(x, y);
}


void Level::setup_start_tiles() {
	start_tile = get_random_passable_tile_from_vector();
	start_display_tile = Coordinate(start_tile.get_coordinate_x() - 3, start_tile.get_coordinate_y() - 3);
}

Coordinate Level::get_start_tile() {
	return start_tile;
}

Coordinate Level::get_start_display_tile() {
	return start_display_tile;
}

//Checks if the coordinate is valid on the map
bool Level::is_valid_coordinate(int x, int y) {
	if (0 <= x && x < kSize && 0 <= y && y < kSize) {
		return true;
	}

	return false;
}

bool Level::is_valid_coordinate(Coordinate coordinate) {
	return (is_valid_coordinate(coordinate.get_coordinate_x(), coordinate.get_coordinate_y()));
}

void Level::load_room_presets() {
	std::fstream filestream;
	filestream.open("../bin/data/graphics/Presets/rooms.txt", std::fstream::in);
	Room current_preset;
	while (filestream >> current_preset) {
		std::cout << "Loading rooms..." << std::endl;
		room_presets.push_back(current_preset);
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
			if (is_valid_coordinate(x, y) && room_tile_index < random_room.size() && get_tile(y, x).get_type() != "floor") {
				map[y][x] = random_room.get_tile(room_tile_index);
			}
			room_tile_index++;
		}
	}
}


void Level::gather_all_passable_tiles() {
	for (int y = 0; y < kSize; y++) {
		for (int x = 0; x < kSize; x++) {
			if (map[y][x].get_passability()) {
				passable_tile_locations.push_back(Coordinate(x, y));
			}
		}
	}
}


Coordinate Level::get_random_passable_tile_from_vector() {
	int index = rand() % (passable_tile_locations.size());
	return passable_tile_locations[index];
}

void Level::generate_random_hall() {
	Coordinate current = get_random_void_tile();
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

	while (is_valid_coordinate(current_tile) && get_tile(y, x).get_type() != "floor") {
		map[y][x] = Tile(true, "floor");
		if (is_valid_coordinate(y + y_wall_modifier, x + x_wall_modifier) && get_tile(y + y_wall_modifier, x + x_wall_modifier).get_type() != "floor") {
			map[y + y_wall_modifier][x + x_wall_modifier] = Tile(false, "wall");
		}
		if (is_valid_coordinate(y - y_wall_modifier, x - x_wall_modifier) && get_tile(y - y_wall_modifier, x - x_wall_modifier).get_type() != "floor") {
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

void Level::instantiate_level() {
	while (percent_passable_tiles() < kPercentPassable) {
		add_random_room_randomly();
		add_random_room_randomly();
		generate_random_hall();
	}
}
