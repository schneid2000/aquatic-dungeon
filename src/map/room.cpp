#include "room.h"

Room::Room() {

}

Room::Room(int new_height, int new_width, std::string tiles) {
	height = new_height;
	width = new_width;
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
			room_tiles.push_back(Tile(false, "ceiling"));
		}
	}
}

int Room::get_height() {
	return height;
}

int Room::get_width() {
	return width;
}

Tile Room::get_tile(int index) {
	if (index >= 0 && index < room_tiles.size()) {
		return room_tiles[index];
	}
}

int Room::size() {
	return room_tiles.size();
}

//Help with changing string input to ints https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
std::istream& operator>>(std::istream &input, Room &room) {
	std::string line;
	std::string room_string;
	std::getline(input, line);


	if (line != "#drf1.0") {
		return input;
	}

	int new_height;
	int new_width;

	std::getline(input, line);
	new_height = (int)line[0] - '0';
	std::getline(input, line);
	new_width = (int)line[0] - '0';
	for (int i = 0; i < new_height; i++) {
		std::getline(input, line);
		room_string += line;
	}
	room = Room(new_height, new_width, room_string);
	
	return input;
}


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