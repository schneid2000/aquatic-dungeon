#include "coordinate.h"

//Empty constructor
Coordinate::Coordinate() {
	
}

//Constructor that takes an x and y value
Coordinate::Coordinate(int new_x, int new_y) {
	x = new_x;
	y = new_y;
}

//Getter for the x coordinate
int Coordinate::get_coordinate_x() {
	return x;
}

//Getter for the y coordinate
int Coordinate::get_coordinate_y() {
	return y;
}

//Setter for the x coordinate
void Coordinate::set_coordinate_x(int new_x) {
	x = new_x;
}

//Setter for the y coordinate
void Coordinate::set_coordinate_y(int new_y) {
	y = new_y;
}

//Setter for both the x and y coordinates
void Coordinate::set_coordinate(int new_x, int new_y) {
	x = new_x;
	y = new_y;
}

//Overload operator for == for Coordinate object
//Coordinates are equal if their x and y values are equal
bool operator== (const Coordinate &first, const Coordinate &second) {
	if (first.x == second.x && first.y == second.y) {
		return true;
	}

	return false;
}