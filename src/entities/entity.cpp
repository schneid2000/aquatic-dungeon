#pragma once

#include "entity.h"



//Gets the euclidean distance between two coordinates
int get_euclidean_distance(Coordinate start, Coordinate end) {
	int x = start.get_coordinate_x() - end.get_coordinate_x();
	int y = start.get_coordinate_y() - end.get_coordinate_y();
	
	//We want the absolute value, so no values should be negative
	if (x < 0) {
		x *= -1;
	}

	if (y < 0) {
		y *= -1;
	}

	//Return the sum of the x and y components
	return x + y;
}




