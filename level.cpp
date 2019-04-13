#include "level.h"

//The default level constructor will create a 20x20 map of normal floor tiles
Level::Level() {
	for (int y = 0; y < kSize; y++) {
		for (int x = 0; x < kSize; x++) {
			map[y][x] = Tile();
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
	srand(time(NULL));
	int y = rand() % 20;
	int x = rand() % 20;

	while (!map[y][x].get_passability()) {
		y = rand() % 20;
		x = rand() % 20;
	}

	return Coordinate(x, y);
}

//Set up the start tile and the display start tile
void Level::setup_start_tiles() {
	start_tile = get_random_passable_tile();
	start_display_tile = Coordinate(start_tile.get_coordinate_x - 3, start_tile.get_coordinate_y - 3);
}
