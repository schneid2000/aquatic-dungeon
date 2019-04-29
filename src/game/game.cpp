#include "game.h"

//Default constructor
Game::Game() {
	level = Level(false);
	level.load_room_presets();
	level.instantiate_level();
	player = Player(level.get_start_tile(), 100, 20);
	mode = "default";
	create_entities();
}

//Getter for the level
Level& Game::get_level() {
	return level;
}

Player& Game::get_player() {
	return player;
}

entt::DefaultRegistry& Game::get_registry() {
	return registry;
}

std::string Game::get_mode() {
	return mode;
}

//Checks if the player is in combat mode
bool Game::in_combat_mode() {
	if (mode == "combat") {
		return true;
	}

	return false;
}

//Setter for the mode variable
void Game::set_mode(std::string new_mode) {
	mode = new_mode;
}

//Create the entities
void Game::create_entities() {
	//The range of enemies to spawn in the level
	int num_entities = (rand() % kMaxAdditionalEntities) + kMinEntities;

	//For the number of entities we want to generate, choose a floor tile to spawn on, and give it a randomy enemy type
	//Then initialize the stats for that enemy
	for (int i = 0; i < num_entities; i++) {
		auto entity = registry.create();
		registry.assign<Location>(entity, level.get_random_passable_tile());
		int type = rand() % kEnemyTypes;
		if (type == 0) {
			registry.assign<Enemy>(entity, 100, 100, 20, "Crabman", "front");
		}
		else if (type == 1) {
			registry.assign<Enemy>(entity, 40, 40, 10, "Octopus", "front");
		}
		else if (type == 2) {
			registry.assign<Enemy>(entity, 20, 20, 10, "Eel", "front");
		}
	}
}


//Checks if an enemy is at the current point
bool Game::is_enemy_at_position(int x, int y) {
	//Get a view of the location components of the enemies
	auto locations = registry.view<Location>();
	
	for (auto entity : locations) {
		if (registry.has<Enemy>(entity)) {
			auto &loc = locations.get(entity);
			//If an enemy in the registry has this location, return true
			if (loc.current_tile.get_coordinate_x() == x && loc.current_tile.get_coordinate_y() == y) {
				return true;
			}
		}
	}

	//No enemies have been found at this location, return false
	return false;
}

bool Game::is_enemy_at_position(Coordinate coordinate) {
	return is_enemy_at_position(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}


//Checks if a space is within the player's attack range
bool Game::is_combat_space(int x, int y) {
	//Shorthand for the player's x and y
	int player_x = player.get_player_x();
	int player_y = player.get_player_y();

	//If the tile is immediately above, below, left, or right, return true
	if (x == player_x && (y == player_y + 1 || y == player_y - 1)) {
		return true;
	}
	else if (y == player_y && (x == player_x + 1 || x == player_x - 1)) {
		return true;
	}

	//The space is not adjacent to the player so return false
	return false;
}

bool Game::is_combat_space(Coordinate coordinate) {
	return is_combat_space(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}


//Takes a 'turn' for all of the enemies in the level
void Game::enemies_action() {
	//The destination is the tile the enemy will end up on
	Coordinate destination;

	//Create views to see enemy locations and stats
	auto locations = registry.view<Location>();
	auto stats = registry.view<Enemy>();

	//Loop through every entity with an Enemy component
	//All entities with an Enemy component should also have a Location component
	for (auto entity : stats) {
		//Retrieve the locations and stats of the enemy
		auto &loc = locations.get(entity);
		auto &dir = stats.get(entity);

		//The default destination should be their current tile
		//So they will maintain their current position if they do not move
		destination = loc.current_tile;

		//The enemy has 3 options:
		//If the enemy is adjacent to the player (r = 1) they will attack the player
		if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) == 1) {
			attack_player(dir.strength);

		} else if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) <= 3 && get_euclidean_distance(loc.current_tile, player.get_current_tile()) > 1) {
			//If the enemy is between 1 and a larger radius they will attempt to move closer to the player
			destination = move_enemy_towards_player(loc.current_tile.get_coordinate_x(), loc.current_tile.get_coordinate_y());

		} else if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) > 3) {
			//If they are too far away they will move randomly
			destination = move_enemy_randomly(loc.current_tile.get_coordinate_x(), loc.current_tile.get_coordinate_y());

		}


		//If the enemy is not next to the player and the destination has changed, change the direction based on the destination 
		if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) > 1 && (loc.current_tile.get_coordinate_x() != destination.get_coordinate_x()) || loc.current_tile.get_coordinate_y() != destination.get_coordinate_y()) {
			dir.direction = determine_direction_relatively(loc.current_tile, destination);
		}
		else if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) == 1) {
			//Otherwise change direction based on the player
			dir.direction = determine_direction_relatively(loc.current_tile, player.get_current_tile());
		}
		loc.current_tile = destination;
	}
}


//Moves an enemy randomly
Coordinate Game::move_enemy_randomly(int current_x, int current_y) {
	//Determine the modifiers for x and y
	int x = (rand() % 3) - 1; // -1 to 1
	int y = 0;
	if (x == 0) {
		y = (rand() % 3) - 1; // -1 to 1
	}

	x += current_x;
	y += current_y;

	int counter = 0;
	while (!level.is_valid_coordinate(x, y) || !level.get_tile(x, y).get_passability() || is_enemy_at_position(x, y) || (x == player.get_player_x() && y == player.get_player_y())) {
		//If 4 attempts have been made (arbitrary number) then just don't move
		if (counter == 4) {
			return Coordinate(current_x, current_y);
		}

		x = current_x;
		y = current_y;

		x = (rand() % 3) - 1;
		y = 0;
		if (x == 0) {
			y = (rand() % 3) - 1;
		}

		x += current_x;
		y += current_y;

		counter++;
	}

	return Coordinate(x, y);
}


Coordinate Game::move_enemy_randomly(Coordinate coordinate) {
	return move_enemy_randomly(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}

std::string Game::determine_direction_relatively(Coordinate start, Coordinate end) {
	if (end.get_coordinate_x() == start.get_coordinate_x() + 1) {
		return "right";
	}
	else if (end.get_coordinate_x() == start.get_coordinate_x() - 1) {
		return "left";
	}
	else if (end.get_coordinate_y() == start.get_coordinate_y() - 1) {
		return "up";
	}
	else {
		return "down";
	}
}


Coordinate Game::get_coordinate_from_pixel(int pixel_x, int pixel_y) {
	int x = pixel_x / 128;
	int y = pixel_y / 128;
	x += (player.get_player_x() - 3);
	y += (player.get_player_y() - 3);

	return Coordinate(x, y);
}

void Game::attack_enemy_at_tile(Coordinate target) {
	auto locations = registry.view<Location>();
	auto stats = registry.view<Enemy>();
	for (auto entity : locations) {
		if (registry.has<Enemy>(entity)) {
			auto &loc = locations.get(entity);
			if (target.get_coordinate_x() == loc.current_tile.get_coordinate_x() && target.get_coordinate_y() == loc.current_tile.get_coordinate_y()) {
				auto &opponent = stats.get(entity);
				opponent.health = opponent.health - player.get_strength();
			}
		}
	}
}




Coordinate Game::move_enemy_towards_player(int current_x, int current_y) {
	int x = current_x;
	int y = current_y;
	int distance = get_euclidean_distance(Coordinate(x, y), player.get_current_tile());
	std::vector<bool> distance_changes;
	std::vector<Coordinate> new_locations;
	new_locations.push_back(Coordinate(x + 1, y));
	new_locations.push_back(Coordinate(x - 1, y));
	new_locations.push_back(Coordinate(x, y + 1));
	new_locations.push_back(Coordinate(x, y - 1));
	new_locations.push_back(Coordinate(x, y));
	distance_changes.push_back(is_distance_reduced(distance, Coordinate(x + 1, y), player.get_current_tile()));
	distance_changes.push_back(is_distance_reduced(distance, Coordinate(x - 1, y), player.get_current_tile()));
	distance_changes.push_back(is_distance_reduced(distance, Coordinate(x, y + 1), player.get_current_tile()));
	distance_changes.push_back(is_distance_reduced(distance, Coordinate(x, y - 1), player.get_current_tile()));
	//This last value represents no movement
	distance_changes.push_back(true);
	int chosen_value = rand() % distance_changes.size();
	while (!distance_changes[chosen_value]) {
		chosen_value = rand() % distance_changes.size();
	}

	return new_locations[chosen_value];
}



bool Game::is_distance_reduced(int initial_d, Coordinate new_start, Coordinate end) {
	int final_distance = get_euclidean_distance(new_start, end);
	if (final_distance < initial_d && level.is_valid_passable_tile(new_start)
		&& !is_enemy_at_position(new_start)
		&& (new_start.get_coordinate_x() != player.get_player_x() || new_start.get_coordinate_y() != player.get_player_y())) {
		return true;
	}

	return false;
}

void Game::attack_player(int damage) {
	player.change_health(damage *  -1);
}


//Shorthand utility function for checking during player movement
bool Game::is_tile_unobstructed(int x, int y) {
	if (level.is_valid_coordinate(x, y)
		&& level.get_tile(x, y).get_passability() && !is_enemy_at_position(x, y)) {
		return true;
	}

	return false;
}

bool Game::is_tile_unobstructed(Coordinate coordinate) {
	return is_tile_unobstructed(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}

std::string Game::get_random_item_type() {
	int type = rand() % kItemTypes;
	if (type == 0) {
		return "Armor";
	}
	else if (type == 1) {
		return "Magic";
	}
	else {
		return "Weapon";
	}
}


std::string Game::generate_random_name(std::string type) {
	//Constant sizes for the prefixes, suffixes, and names for random selection
	const int prefix_size = 15;
	const int suffix_size = 6;
	const int name_size = 5;
	std::string prefixes[prefix_size] = { "Aquatic", "Burning", "Shining", "Gleaming", "Crystalline",
			"Rough", "Smooth", "Oceanic", "Soaked", "Drenched",
			"Crabman's", "Scavenger's", "Explorer's", "Geoff's", "Carl's" };
	std::string suffixes[suffix_size] = { "of the Ocean", "of the Lake", "of the River",
			"of the Sand", "of the Octopus", "of the Eel" };

	std::string prefix_string = "";
	std::string suffix_string = "";
	std::string name_string = "";

	int prefix = rand() % (2 * prefix_size);
	int suffix = rand() % (2 * suffix_size);
	int name = 0;

	//Having a prefix is not guaranteed, so only include it if one was chosen
	if (prefix < prefix_size) {
		prefix_string = prefixes[prefix];
	}

	//Having a suffix is not guaranteed, so only include it if one was chosen
	if (suffix < suffix_size) {
		suffix_string = suffixes[suffix];
	}

	//Based on the item type, choose a different item name
	if (type == "Armor") {
		std::string armor_names[name_size] = { "Shield", "Buckler", "Kite Shield", "Wooden Shield", "Knight's Shield"};
		name = rand() % (name_size);
		name_string = armor_names[name];

		return prefix_string + " " + name_string + " " + suffix_string;
	} else if (type == "Magic") {
		std::string magic_names[name_size] = {"Orb", "Sphere", "Stone", "Crystal", "Gem"};
		name = rand() % (name_size);
		name_string = magic_names[name];

		return prefix_string + " " + name_string + " " + suffix_string;
	} else if (type == "Weapon") {
		std::string weapon_names[name_size] = {"Sword", "Blade", "Dagger", "Broadsword", "Greatsword"};
		name = rand() % (name_size);
		name_string = weapon_names[name];

		return prefix_string + " " + name_string + " " + suffix_string;
	} else {
		return "Random Item Bag";
	}
}

//Gets a random image name based on the item type
std::string Game::get_random_image_name(std::string type) {
	if (type == "Armor") {
		std::string armor_names[kArmorImages] = { "shield" };
		return armor_names[rand() % kArmorImages];
	} else if (type == "Magic") {
		std::string magic_names[kMagicImages] = { "orb_1", "orb_2", "orb_3" };
		return magic_names[rand() % kMagicImages];
	} else if (type == "Weapon") {
		std::string weapon_names[kWeaponImages] = { "dagger", "sword" };
		return weapon_names[rand() % kWeaponImages];
	} else {
		return "random_item_bag";
	}
}