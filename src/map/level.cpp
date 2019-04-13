#include "level.h"

//The default level constructor will create a 20x20 map of normal floor tiles
Level::Level() {
	/*for (int y = 0; y < kSize; y++) {
		for (int x = 0; x < kSize; x++) {
			map[y][x].set_passability(false);
		}
	}*/
}

Level::Level(bool passable) {
	if (passable) {
		for (int y = 0; y < kSize; y++) {
			for (int x = 0; x < kSize; x++) {
				map[y][x] = Tile(true, "bridge_v");
			}
		}
	}
}

//Gets a specific map tile by the coordinate
Tile Level::get_tile(int y, int x) {
	return map[y][x];
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
	int y = rand() % 20;
	int x = rand() % 20;

	while (!map[y][x].get_passability()) {
		y = rand() % 20;
		x = rand() % 20;
	}

	return Coordinate(x, y);
}


void Level::setup_start_tiles() {
	start_tile = get_random_passable_tile();
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

void Level::load_room_presets() {
	std::fstream filestream;
	filestream.open("../bin/data/graphics/Presets/rooms.txt", std::fstream::in);
	Room current_preset;
	while (filestream >> current_preset) {
		room_presets.push_back(current_preset);
	}
}

void Level::add_random_room_randomly() {
	if (room_presets.size() == 0) {
		return;
	}
	int index = rand() % (room_presets.size());
	Coordinate random_tile = get_random_passable_tile();
	Room random_room = room_presets[index];
	int room_tile_index = 0;
	for (int y = random_tile.get_coordinate_y(); y < random_tile.get_coordinate_y() + random_room.get_height(); y++) {
		for (int x = random_tile.get_coordinate_x(); x < random_tile.get_coordinate_x() + random_room.get_width(); x++) {
			if (is_valid_coordinate(x, y) && room_tile_index < random_room.size()) {
				std::cout << "is this line being run?" << std::endl;
				map[y][x] = random_room.get_tile(room_tile_index);
			}
			room_tile_index++;
		}
	}
}
