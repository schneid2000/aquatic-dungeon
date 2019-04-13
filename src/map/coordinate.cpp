#include "coordinate.h"

Coordinate::Coordinate() {
	Coordinate(0, 0);
}

Coordinate::Coordinate(int new_x, int new_y) {
	x = new_x;
	y = new_y;
}

int Coordinate::get_coordinate_x() {
	return x;
}

int Coordinate::get_coordinate_y() {
	return y;
}

void Coordinate::set_coordinate_x(int new_x) {
	x = new_x;
}

void Coordinate::set_coordinate_y(int new_y) {
	y = new_y;
}

void Coordinate::set_coordinate(int new_x, int new_y) {
	x = new_x;
	y = new_y;
}