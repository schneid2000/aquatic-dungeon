#pragma once

#include "../map/coordinate.h"
#include <string>

//Location of the enemy/item (if in the inventory, contains the relative location of the item)
struct Location {
	Coordinate current_tile;
};

//Enemy stats
struct Enemy {
	int health;
	int total_health;
	int strength;
	std::string name;
	std::string direction;
};


//Item stats
struct Item {
	std::string name;
	std::string type;
	std::string image;
};


//Modifiers for items that go in specified slots
struct Equipment {
	int melee_modifier;
	int armor_strength;
	int magic_modifier;
};

//Healing for health items
struct Healing {
	int health;
};

//Property of items in the player's inventory
struct InventorySlot {
	int slot_id;
	std::string type_restriction;
};

//Boss stats
struct Boss {
	Coordinate start;
	std::string name;
	int current_health;
	int total_health;
	int strength;
};

//Get the euclidean distance between two coordinates
int get_euclidean_distance(Coordinate start, Coordinate end);

