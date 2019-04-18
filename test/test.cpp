#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../src/map/coordinate.h"
#include "../src/map/level.h"
#include "../src/map/room.h"
#include "../src/map/tile.h"

//Coordinate tests

TEST_CASE("Constructor correctly assigns x and y") {
	Coordinate point = Coordinate(4, 2);
	REQUIRE(point.get_coordinate_x() == 4);
	REQUIRE(point.get_coordinate_y() == 2);
}

TEST_CASE("Setting a new x coordinate") {
	Coordinate point = Coordinate(4, 2);
	point.set_coordinate_x(3);
	REQUIRE(point.get_coordinate_x() == 3);
}

TEST_CASE("Setting a new y coordinate") {
	Coordinate point = Coordinate(4, 2);
	point.set_coordinate_y(3);
	REQUIRE(point.get_coordinate_y() == 3);
}



// Tile tests

TEST_CASE("Default Tile Constructor returns an impassable Ceiling tile") {
	Tile tile = Tile();
	REQUIRE(!tile.get_passability());
	REQUIRE(tile.get_type() == "ceiling");
}

TEST_CASE("Tile Constructor works properly") {
	Tile tile = Tile(true, "floor");
	REQUIRE(tile.get_passability());
	REQUIRE(tile.get_type() == "floor");
}

TEST_CASE("Setting a new passability on a tile") {
	Tile tile = Tile(true, "floor");
	tile.set_passability(false);
	REQUIRE(!tile.get_passability());
}


// Room tests

TEST_CASE("Room constructor works properly") {
	Room room = Room(7, 5, "HHHHHH---HH---HH---HH---HH---HHHHHH");
	REQUIRE(room.get_height() == 7);
	REQUIRE(room.get_width() == 5);
	REQUIRE(room.get_tile(0).get_type() == "wall");
}

TEST_CASE("Gets size of room correctly") {
	Room room = Room(7, 5, "HHHHHH---HH---HH---HH---HH---HHHHHH");
	REQUIRE(room.size() == 35);
}


// Level tests

TEST_CASE("Constructor works properly given true") {
	Level level = Level(true);
	REQUIRE(level.get_tile(0, 0).get_type() == "floor");
	REQUIRE(level.get_tile(0, 0).get_passability());
}

TEST_CASE("Constructor works properly given false") {
	Level level = Level(false);
	REQUIRE(level.get_tile(0, 0).get_type() == "ceiling");
	REQUIRE(!level.get_tile(0, 0).get_passability());
}

TEST_CASE("Setting a specific tile in a Level") {
	Level level = Level(false);
	level.set_tile(1, 0, Tile(true, "floor"));
	REQUIRE(level.get_tile(1, 0).get_type() == "floor");
}

TEST_CASE("Getting a specific tile with a Coordinate") {
	Level level = Level(false);
	level.set_tile(1, 0, Tile(true, "floor"));
	REQUIRE(level.get_tile(Coordinate(1, 0)).get_type() == "floor");
}

TEST_CASE("Getting a count of all passable tiles in a Level") {
	Level level = Level(false);
	level.set_tile(1, 0, Tile(true, "floor"));
	level.set_tile(0, 1, Tile(true, "floor"));
	REQUIRE(level.count_all_passable_tiles() == 2);
}
