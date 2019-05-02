#pragma once


class Coordinate {
private:
	int x;
	int y;

public: 
	//Empty constructor
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

	//https://www.learncpp.com/cpp-tutorial/96-overloading-the-comparison-operators/ help with overloading operators
	friend bool operator==(const Coordinate &first, const Coordinate &second);
};