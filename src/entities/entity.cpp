#pragma once

#include "entity.h"




int get_euclidean_distance(Coordinate start, Coordinate end) {
	int x = start.get_coordinate_x() - end.get_coordinate_x();
	int y = start.get_coordinate_y() - end.get_coordinate_y();
	if (x < 0) {
		x *= -1;
	}

	if (y < 0) {
		y *= -1;
	}

	return x + y;
}




