#pragma once

#include <entt/entt.hpp>
#include "../map/coordinate.h"
#include <string>

struct Location {
	Coordinate current_tile;
};

struct Enemy {
	int health;
	int total_health;
	int strength;
	std::string name;
	std::string direction;
};

struct Item {
	std::string name;
	std::string type;
};

struct Equipment {
	int melee_modifier;
	int armor_strength;
	int magic_modifier;
};


