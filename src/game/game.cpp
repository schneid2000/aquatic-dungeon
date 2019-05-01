#include "game.h"

//Default constructor
Game::Game() {
	level = Level(false);
	level.load_room_presets();
	level.instantiate_level();
	player = Player(level.get_start_tile(), 100, 20);
	mode = "default";
	boss_mode = false;
	victory = false;
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

bool Game::is_boss_mode() {
	return boss_mode;
}

bool Game::get_victory() {
	return victory;
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
		} else if (type == 1) {
			registry.assign<Enemy>(entity, 40, 40, 10, "Octopus", "front");
		} else if (type == 2) {
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

	auto bosses = registry.view<Boss>();

	//Go through bosses and check for valid boss boxes based on the boss name
	for (auto boss : bosses) {
		auto &boss_stats = bosses.get(boss);
		if ((boss_stats.start.get_coordinate_x() == x && boss_stats.start.get_coordinate_y() == y) 
			|| (boss_stats.start.get_coordinate_x() == x && boss_stats.start.get_coordinate_y() - 1 == y) 
			&& boss_stats.name == "Cuttlefish") {
			return true;
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
	} else if (y == player_y && (x == player_x + 1 || x == player_x - 1)) {
		return true;
	}

	//The space is not adjacent to the player so return false
	return false;
}

//Checks for a combat space given a coordinate
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
		} else if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) == 1) {
			//Otherwise change direction based on the player
			dir.direction = determine_direction_relatively(loc.current_tile, player.get_current_tile());
		}
		loc.current_tile = destination;
	}

	if (boss_mode) {
		auto bosses = registry.view<Boss>();
		for (auto boss : bosses) {
			auto &boss_stats = bosses.get(boss);
			boss_action(boss_stats.name);
		}
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


//Move an enemy randomly given their current position as a coordinate
Coordinate Game::move_enemy_randomly(Coordinate coordinate) {
	return move_enemy_randomly(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}


//Determine what direction an enemy should face (for display)
std::string Game::determine_direction_relatively(Coordinate start, Coordinate end) {
	if (end.get_coordinate_x() == start.get_coordinate_x() + 1) {
		return "right";
	} else if (end.get_coordinate_x() == start.get_coordinate_x() - 1) {
		return "left";
	} else if (end.get_coordinate_y() == start.get_coordinate_y() - 1) {
		return "up";
	} else {
		return "down";
	}
}


//Get the specific map coordinate from where the user clicked
Coordinate Game::get_coordinate_from_pixel(int pixel_x, int pixel_y) {
	int x = pixel_x / 128;
	int y = pixel_y / 128;
	x += (player.get_player_x() - 3);
	y += (player.get_player_y() - 3);

	return Coordinate(x, y);
}


//Get the relative coordinate based on where the user clicked
Coordinate Game::get_relative_coordinate_from_pixel(int pixel_x, int pixel_y) {
	int x = pixel_x / 128;
	int y = pixel_y / 128;

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
				opponent.health = opponent.health - (player.get_strength() + get_modifier(0));
				if (opponent.health <= 0) {
					get_registry().remove<Enemy>(entity);
					std::string type = get_random_item_type();
					get_registry().assign<Item>(entity, generate_random_name(type), type, get_random_image_name(type));
					if (type == "Healing") {
						registry.assign<Healing>(entity, 50);
					}
					else {
						get_registry().assign<Equipment>(entity, get_random_value_by_type(type, "Weapon"), get_random_value_by_type(type, "Armor"), get_random_value_by_type(type, "Magic"));
					}
				}
			}
		}
	}

	auto bosses = registry.view<Boss>();
	for (auto boss : bosses) {
		auto &boss_stats = bosses.get(boss);
		if ((target.get_coordinate_x() == boss_stats.start.get_coordinate_x() && target.get_coordinate_y() == boss_stats.start.get_coordinate_y())
			|| (target.get_coordinate_x() == boss_stats.start.get_coordinate_x() && target.get_coordinate_y() == boss_stats.start.get_coordinate_y() - 1) 
			&& boss_stats.name == "Cuttlefish") {
			boss_stats.current_health = boss_stats.current_health - (player.get_strength() + get_modifier(0));
			if (boss_stats.current_health <= 0) {
				registry.destroy(boss);
				victory = true;
				player.change_health(player.get_total_health());
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
	int health_adjustment = (damage * -1) + get_modifier(1);
	if (health_adjustment > 0) {
		health_adjustment = 0;
	}
	player.change_health(health_adjustment);
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



int Game::get_slot_from_relative_coordinate(Coordinate coordinate) {
	if (coordinate.get_coordinate_y() == 1) {
		return (coordinate.get_coordinate_x() - 1) / 2;
	} else if (coordinate.get_coordinate_y() == 4 && coordinate.get_coordinate_x() >= 2 && coordinate.get_coordinate_x() < 5) {
		return coordinate.get_coordinate_x() + 1;
	} else if (coordinate.get_coordinate_y() == 5 && coordinate.get_coordinate_x() >= 2 && coordinate.get_coordinate_x() < 5) {
		return coordinate.get_coordinate_x() + 4;
	}

	return -1;
}

Coordinate Game::get_coordinate_from_slot(int slot) {
	if (slot >= 0 && slot < 3) {
		return Coordinate(slot + (slot + 1), 1);
	} else if (slot >= 3 && slot < 6) {
		return Coordinate(slot - 1, 4);
	}
	else if (slot >= 6) {
		return Coordinate(slot - 4, 5);
	}
}



int Game::get_random_value_by_type(std::string type, std::string compare_type) {
	if (type == compare_type) {
		if (type == "Armor") {
			return ((rand() % 10) + 1);
		}
		else if (type == "Weapon") {
			return ((rand() % 20) + 1);
		}
		else {
			return ((rand() % 10) + 1);
		}
	}
	
	return 0;
}


std::string Game::get_random_item_type() {
	int type = rand() % kItemTypes;
	if (type == 0) {
		return "Armor";
	} else if (type == 1) {
		return "Magic";
	} else if (type == 2) {
		return "Weapon";
	} else {
		return "Healing";
	} 
}


std::string Game::generate_random_name(std::string type) {
	if (type == "Healing") {
		return "Healing Potion";
	}

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
	} else if (type == "Healing") {
		return "healing_potion";
	} else {
		return "random_item_bag";
	}
}

void Game::check_to_add_item() {
	if (player.is_inventory_full()) {
		return;
	}
	auto items = registry.view<Item>();
	auto locations = registry.view<Location>();
	for (auto entity : items) {
		if (registry.has<Location>(entity) && !registry.has<InventorySlot>(entity)) {
			auto &loc = locations.get(entity);
			auto &stats = items.get(entity);
			if (loc.current_tile.get_coordinate_x() == player.get_player_x()
				&& loc.current_tile.get_coordinate_y() == player.get_player_y()) {
				registry.assign<InventorySlot>(entity, player.get_first_empty_slot(), stats.type);
				registry.replace<Location>(entity, player.coord_of_first_empty_slot());
				player.occupy_slot(player.get_first_empty_slot());
			}
		}
	}
}

int Game::get_modifier(int slot) {
	if (slot > 2 || slot < 0) {
		return 0;
	}
	Coordinate destination = get_coordinate_from_slot(slot);
	auto inventory_items = registry.view<InventorySlot>();
	auto locations = registry.view<Location>();
	auto equipment = registry.view<Equipment>();
	for (auto item : inventory_items) {
		auto &loc = locations.get(item);
		if (registry.has<Equipment>(item)) {
			auto &equip_stats = equipment.get(item);
			if (destination.get_coordinate_x() == loc.current_tile.get_coordinate_x()
				&& destination.get_coordinate_y() == loc.current_tile.get_coordinate_y()) {
				if (slot == 0) {
					return equip_stats.melee_modifier;
				} else if (slot == 1) {
					return equip_stats.armor_strength;
				} else if (slot == 2) {
					return equip_stats.magic_modifier;
				}
			}
		}
	}

	return 0;
}



void Game::attempt_boss_spawn() {
	if (level.get_tile(player.get_current_tile()).get_type() == "boss_spawn") {
		boss_mode = true;
		//Replace the boss spawn tile with a floor tile
		level.set_tile(player.get_player_x(), player.get_player_y(), Tile(true, "no_spawn_floor"));

		//Wall off the entrances to the boss room and destroy any entities that were there
		for (int y = player.get_player_y() - 3; y < player.get_player_y() + 4; y++) {
			for (int x = player.get_player_x() - 3; x < player.get_player_x() + 4; x++) {
				if (level.is_valid_coordinate(x, y) && level.get_tile(x, y).get_type() == "boss_gateway") {
					auto locations = registry.view<Location>();
					for (auto entity : locations) {
						auto &loc = locations.get(entity);
						if ((registry.has<Item>(entity) && !registry.has<InventorySlot>(entity)) || registry.has<Enemy>(entity) 
							&& loc.current_tile.get_coordinate_x() == x && loc.current_tile.get_coordinate_y() == y) {
							registry.destroy(entity);
						}
					}
					level.set_tile(x, y, Tile(false, "boss_wall"));
				}
			}
		}

		//Select a random boss (although there's only 1 type right now)
		int boss = rand() % kBossTypes;
		
		//0 - Cuttlefish
		if (boss == 0) {
			auto boss = registry.create();
			registry.assign<Boss>(boss, Coordinate(player.get_player_x(), player.get_player_y() - 1), "Cuttlefish", 200, 200, 50);
			level.set_tile(player.get_player_x(), player.get_player_y() - 1, Tile(false, "no_spawn_floor"));
			level.set_tile(player.get_player_x(), player.get_player_y() - 2, Tile(false, "no_spawn_floor"));
		}
	}




}

//Takes a 'turn' for bosses given the name of the boss
void Game::boss_action(std::string name) {
	auto &bosses = registry.view<Boss>();
	for (auto boss : bosses) {
		auto &boss_stats = bosses.get(boss);
		if (name == boss_stats.name && name == "Cuttlefish") {
			update_arms_in_radius(boss_stats.start, 3, boss_stats.strength);
			spread_from_center(level.get_random_passable_tile_in_radius(boss_stats.start, 3), 5);
		}
	}
	
}

void Game::spread_from_center(Coordinate center, int distance) {
	int attempts = 0;
	Coordinate current = Coordinate(center.get_coordinate_x(), center.get_coordinate_y());
	std::string direction = orient_towards_player(center);
	int x_modifier = 0;
	int y_modifier = 0;
	if (direction == "right") {
		x_modifier = 1;
	}
	else if (direction == "left") {
		x_modifier = -1;
	}
	else if (direction == "up") {
		y_modifier = -1;
	}
	else if (direction == "down") {
		y_modifier = 1;
	}
	for (int i = 0; i < distance; i++) {
		if (level.is_valid_passable_tile(current)) {
			if ((direction == "up" || direction == "down") && level.get_tile(current).get_type() == "no_spawn_floor") {
				level.set_tile(current.get_coordinate_x(), current.get_coordinate_y(), Tile(true, "arm_1_v"));
			}

			if ((direction == "left" || direction == "right") && level.get_tile(current).get_type() == "no_spawn_floor") {
				level.set_tile(current.get_coordinate_x(), current.get_coordinate_y(), Tile(true, "arm_1_h"));
			}

			current = Coordinate(current.get_coordinate_x() + x_modifier, current.get_coordinate_y() + y_modifier);
			
		}
	}

	
}


std::string Game::orient_towards_player(Coordinate center) {
	int y_distance = center.get_coordinate_y() - player.get_player_y();
	int x_distance = center.get_coordinate_x() - player.get_player_x();
	if (x_distance < 0) {
		x_distance *= -1;
	}
	if (y_distance < 0) {
		y_distance *= -1;
	}
	if (y_distance < x_distance) {
		if (center.get_coordinate_x() > player.get_player_x()) {
			return "left";
		} else if (center.get_coordinate_x() <= player.get_player_x()) {
			return "right";
		}
	} else {
		if (center.get_coordinate_y() > player.get_player_y()) {
			return "up";
		} else if (center.get_coordinate_y() <= player.get_player_y()) {
			return "down";
		}
	}

	if (x_distance == y_distance) {
		return "right";
	}
}


void Game::update_arms_in_radius(Coordinate center, int radius, int strength) {
	for (int y = center.get_coordinate_y() - radius; y <= center.get_coordinate_y() + radius; y++) {
		for (int x = center.get_coordinate_x() - radius; x <= center.get_coordinate_x() + radius; x++) {
			if (level.is_valid_coordinate(x, y)) {
				if (level.get_tile(x, y).get_type() == "arm_1_v") {
					level.set_tile(x, y, Tile(true, "arm_2_v"));
				} else if (level.get_tile(x, y).get_type() == "arm_1_h") {
					level.set_tile(x, y, Tile(true, "arm_2_h"));
				} else if (level.get_tile(x, y).get_type() == "arm_2_v") {
					if (x == player.get_player_x() && y == player.get_player_y()) {
						if ((-1 * strength) + get_modifier(1) < 0) {
							player.change_health((-1 * strength) + get_modifier(1));
						}
					}
					level.set_tile(x, y, Tile(false, "arm_3_v"));
				} else if (level.get_tile(x, y).get_type() == "arm_2_h") {
					if (x == player.get_player_x() && y == player.get_player_y()) {
						if ((-1 * strength) + get_modifier(1) < 0) {
							player.change_health((-1 * strength) + get_modifier(1));
						}
					}
					level.set_tile(x, y, Tile(false, "arm_3_h"));
				} else if (level.get_tile(x, y).get_type() == "arm_3_v" || level.get_tile(x, y).get_type() == "arm_3_h") {
					level.set_tile(x, y, Tile(true, "no_spawn_floor"));
				}
			}
		}
	}
}