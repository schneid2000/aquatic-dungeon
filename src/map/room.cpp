#include "room.h"

//Empty constructor
Room::Room() {

}


//Constructor that takes the height and width of the room, and a string representing the tiles
Room::Room(int new_height, int new_width, std::string tiles) {
	//Assign the height and width
	height = new_height;
	width = new_width;

	//Go through the string of characters and convert them to the proper tiles
	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i] == 'H') {
			room_tiles.push_back(Tile(false, "wall"));
		} else if (tiles[i] == '-') {
			room_tiles.push_back(Tile(true, "floor"));
		} else if (tiles[i] == '=') {
			room_tiles.push_back(Tile(false, "water"));
		} else if (tiles[i] == '+') {
			room_tiles.push_back(Tile(true, "bridge_v"));
		} else if (tiles[i] == 'x') {
			room_tiles.push_back(Tile(true, "bridge_h"));
		} else {
			//Any ceiling or otherwise unrecognized tile should become a ceiling tile
			room_tiles.push_back(Tile(false, "ceiling"));
		}
	}
}

//Getter for the room height
int Room::get_height() {
	return height;
}

//Getter for the room width
int Room::get_width() {
	return width;
}

//Getter for a particular tile from the tile vector
Tile Room::get_tile(int index) {
	//Only attempt to get a tile if the index is valid
	if (index >= 0 && index < room_tiles.size()) {
		return room_tiles[index];
	}
}

//Get the size of the room
int Room::size() {
	return room_tiles.size();
}

//Overloaded input stream to allow input of rooms from filestream
//Help with changing string input to ints https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
std::istream& operator>>(std::istream &input, Room &room) {
	std::string line;
	std::string room_string;

	//Take in a line
	std::getline(input, line);

	//Do not continue if the file format is not included
	if (line != "#drf1.0") {
		return input;
	}

	int new_height;
	int new_width;

	//Take in a line, assign it to the height
	std::getline(input, line);
	new_height = (int)line[0] - '0';

	//Take in another line, assign it to the width
	std::getline(input, line);
	new_width = (int)line[0] - '0';

	//Create one long string out of the lines of text
	for (int i = 0; i < new_height; i++) {
		std::getline(input, line);
		room_string += line;
	}
	
	//Create a room with these values
	room = Room(new_height, new_width, room_string);
	
	return input;
}

//Overloaded output stream to print rooms to a file
//Unfinished
std::ostream& operator<<(std::ostream &output, const Room &room) {
	output << "#drf1.0" << "\n";
	output << const_cast<Room&>(room).get_height() << "\n";
	output << const_cast<Room&>(room).get_width() << "\n";
	for (int i = 0; i < const_cast<Room&>(room).get_height() * const_cast<Room&>(room).get_width(); i++) {
		Tile current_tile = const_cast<Room&>(room).get_tile(i);
		if (current_tile.get_type() == "wall") {
			output << "H";
		}
		else if (current_tile.get_type() == "floor") {
			output << "-";
		}
		else if (current_tile.get_type() == "water") {
			output << "=";
		}
		else if (current_tile.get_type() == "bridge_v") {
			output << "+";
		}
		else if (current_tile.get_type() == "bridge_h") {
			output << "x";
		}
		else {
			output << "0";
		}
	}
	
	return output;
}