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

