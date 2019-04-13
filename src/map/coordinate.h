#pragma once


class Coordinate {
private:
	int x;
	int y;

public: 
	//Creates a default point at 0, 0
	Coordinate();

	//Constructor that takes in the values
	Coordinate(int new_x, int new_y);

	//Getters for the x and y
	int get_coordinate_x();
	int get_coordinate_y();

	//Setters for the x and y
	void set_coordinate_x(int new_x);
	void set_coordinate_y(int new_y);
	void set_coordinate(int new_x, int new_y);
};