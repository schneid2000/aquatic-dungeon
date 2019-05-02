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

//Getter for the player
Player& Game::get_player() {
	return player;
}

//Getter for the registry
entt::DefaultRegistry& Game::get_registry() {
	return registry;
}

//Getter for the mode
std::string Game::get_mode() {
	return mode;
}

//Checks if the game is in boss mode
bool Game::is_boss_mode() {
	return boss_mode;
}

//Get the victory condition
bool Game::get_victory() {
	return victory;
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
			if (loc.current_tile == Coordinate(x, y)) {
				return true;
			}
		}
	}

	auto bosses = registry.view<Boss>();

	//Go through bosses and check for valid boss boxes based on the boss name
	for (auto boss : bosses) {
		auto &boss_stats = bosses.get(boss);
		if ((boss_stats.start == Coordinate(x, y)) 
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
			destination = move_enemy_towards_player(loc.current_tile);

		} else if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) > 3) {
			//If they are too far away they will move randomly
			destination = move_enemy_randomly(loc.current_tile);

		}


		//If the enemy is not next to the player and the destination has changed, change the direction based on the destination 
		if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) > 1 && !(destination == loc.current_tile)) {
			dir.direction = determine_direction_relatively(loc.current_tile, destination);
		} else if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) == 1) {
			//Otherwise change direction based on the player
			dir.direction = determine_direction_relatively(loc.current_tile, player.get_current_tile());
		}
		loc.current_tile = destination;
	}

	//In boss mode, give the boss an action as well
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

	//x and y now become our new theoretical x and y
	x += current_x;
	y += current_y;

	int counter = 0;
	while (!is_tile_unobstructed(x, y) || (Coordinate(x, y) == player.get_current_tile())) {
		//If 4 attempts have been made (arbitrary number) then just don't move
		if (counter == 4) {
			return Coordinate(current_x, current_y);
		}

		x = (rand() % 3) - 1;
		y = 0;
		if (x == 0) {
			y = (rand() % 3) - 1;
		}

		x += current_x;
		y += current_y;

		counter++;
	}

	//Return the new coordinate
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


//Get the specific map coordinate from where the user clicked (coordinates represent a 128x128 tile)
Coordinate Game::get_coordinate_from_pixel(int pixel_x, int pixel_y) {
	int x = pixel_x / 128;
	int y = pixel_y / 128;
	x += (player.get_player_x() - 3);
	y += (player.get_player_y() - 3);

	return Coordinate(x, y);
}


//Get the relative coordinate based on where the user clicked (coordinates represent a 128x128 tile)
Coordinate Game::get_relative_coordinate_from_pixel(int pixel_x, int pixel_y) {
	int x = pixel_x / 128;
	int y = pixel_y / 128;

	return Coordinate(x, y);
}



//Attempt to attack an enemy at the given coordinate
void Game::attack_enemy_at_tile(Coordinate target) {
	auto locations = registry.view<Location>();
	auto stats = registry.view<Enemy>();
	
	//Search through enemies
	for (auto entity : locations) {
		if (registry.has<Enemy>(entity)) {
			auto &loc = locations.get(entity);
			
			//If an enemy shares the target tile, attempt to attack it
			if (target == loc.current_tile) {
				auto &opponent = stats.get(entity);

				//Adjust its health by the player strength plus the player's Weapon modifier (slot 0)
				opponent.health = opponent.health - (player.get_strength() + get_modifier(0));

				//If the opponent has no health, remove it and make it drop an item
				if (opponent.health <= 0) {
					get_registry().remove<Enemy>(entity);
					std::string type = get_random_item_type();
					get_registry().assign<Item>(entity, generate_random_name(type), type, get_random_image_name(type));

					//If the random type the item gets is Healing, make a Healing item
					if (type == "Healing") {
						registry.assign<Healing>(entity, 50);
					}
					//Otherwise make an Equipment item
					else {
						get_registry().assign<Equipment>(entity, get_random_value_by_type(type, "Weapon"), get_random_value_by_type(type, "Armor"), get_random_value_by_type(type, "Magic"));
					}
				}
			}
		}
	}

	//The player may also be attempting to attack a boss
	auto bosses = registry.view<Boss>();

	//Search through bosses
	for (auto boss : bosses) {
		auto &boss_stats = bosses.get(boss);
		//For the Cuttlefish boss, its start tile and the tile above it represent attackable tiles
		if (target == boss_stats.start && boss_stats.name == "Cuttlefish"
			|| (target.get_coordinate_x() == boss_stats.start.get_coordinate_x() && target.get_coordinate_y() == boss_stats.start.get_coordinate_y() - 1)) {

			//Remove health equal to the player's strength plus their Weapon modifier (slot 0)
			boss_stats.current_health = boss_stats.current_health - (player.get_strength() + get_modifier(0));

			//If the boss runs out of health, set victory to true since the player has won, destroy the boss, and restore the player's health
			if (boss_stats.current_health <= 0) {
				registry.destroy(boss);
				victory = true;
				player.change_health(player.get_total_health());
			}
		}
	}
}



//Attempts to move an enemy closer to a player
//This function receives the enemy's current x and y coordinates
//The enemy will choose randomly from all options that move it closer to the player AND not moving
Coordinate Game::move_enemy_towards_player(int current_x, int current_y) {
	int x = current_x;
	int y = current_y;

	//This is the initial distance between the enemy and the player
	int distance = get_euclidean_distance(Coordinate(x, y), player.get_current_tile());
	std::vector<bool> distance_changes;
	std::vector<Coordinate> new_locations;

	//These coordinates represent the tiles directly above, below, and to the left and right of the enemy
	new_locations.push_back(Coordinate(x + 1, y));
	new_locations.push_back(Coordinate(x - 1, y));
	new_locations.push_back(Coordinate(x, y + 1));
	new_locations.push_back(Coordinate(x, y - 1));

	//Add a true value if the distance between the enemy and the player is decreased, add a false otherwise
	for (int i = 0; i < new_locations.size(); i++) {
		distance_changes.push_back(is_distance_reduced(distance, new_locations[i], player.get_current_tile()));
	}
	//This last value represents no movement
	new_locations.push_back(Coordinate(x, y));
	distance_changes.push_back(true);

	//Choose a true value from the distance change vector
	int chosen_value = rand() % distance_changes.size();
	while (!distance_changes[chosen_value]) {
		chosen_value = rand() % distance_changes.size();
	}

	//Return the random first true value from the distance change vector
	return new_locations[chosen_value];
}

//Move the enemy towards a player, given the enemy's current coordinate
Coordinate Game::move_enemy_towards_player(Coordinate coordinate) {
	return move_enemy_towards_player(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}


//Returns true if the distance between an enemy and a player is reduced (alongside some other conditions)
//Takes in the initial distance between the two, a theoretical new starting coordinate, and the end coordinate (usually the player)
bool Game::is_distance_reduced(int initial_d, Coordinate new_start, Coordinate end) {
	//Get the distance between the new starting and ending point
	int final_distance = get_euclidean_distance(new_start, end);

	//If the distance is smaller AND the theoretical new starting point is a tile the enemy can walk on,
	//then return true
	if (final_distance < initial_d && level.is_valid_passable_tile(new_start)
		&& !is_enemy_at_position(new_start)
		&& (!(new_start == player.get_current_tile()))) {
		return true;
	}

	//If any or all of the conditions are not filled, return false
	return false;
}


//An enemy attacks the player
//Takes in the damage done (usually the strength value of the attacking enemy)
void Game::attack_player(int damage) {
	//The damage done is the damage value (as a negative), reduced by the player's armor modifier (slot 1)
	int health_adjustment = (damage * -1) + get_modifier(1);

	//If the player's armor modifier is greater than the damage done, 
	//do not let the damage done become a positive value
	if (health_adjustment > 0) {
		health_adjustment = 0;
	}

	//Adjust the player's health by the new value
	player.change_health(health_adjustment);
}


//Shorthand utility function for checking during player movement
bool Game::is_tile_unobstructed(int x, int y) {

	//If the coordinate exists on the map, is passable, and there is no enemy there, 
	//then the tile is considered unobstructed
	if (level.is_valid_coordinate(x, y)
		&& level.get_tile(x, y).get_passability() && !is_enemy_at_position(x, y)) {
		return true;
	}

	return false;
}

//Checks if the tile is unobstructed given the coordinate
bool Game::is_tile_unobstructed(Coordinate coordinate) {
	return is_tile_unobstructed(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}


//Gets the integer slot value of the player's inventory (0 - 8) from the relative coordinate of the screen
int Game::get_slot_from_relative_coordinate(Coordinate coordinate) {
	//The weapon, armor, and magic slots (0 - 2) are in (1, 1), (3, 1), and (5, 1) respectively
	if (coordinate.get_coordinate_y() == 1 && coordinate.get_coordinate_x() % 2 != 0) {
		return (coordinate.get_coordinate_x() - 1) / 2;

		//The rest of the slots are x = [3, 5] on y = [4, 5]
	} else if (coordinate.get_coordinate_y() == 4 && coordinate.get_coordinate_x() >= 2 && coordinate.get_coordinate_x() < 5) {
		return coordinate.get_coordinate_x() + 1;
	} else if (coordinate.get_coordinate_y() == 5 && coordinate.get_coordinate_x() >= 2 && coordinate.get_coordinate_x() < 5) {
		return coordinate.get_coordinate_x() + 4;
	}

	return -1;
}


//Returns the relative Coordinate on the screen given the integer value of the inventory slot (0 - 8)
Coordinate Game::get_coordinate_from_slot(int slot) {
	//This represents the weapon, armor, and magic slots (0 - 2)
	if (slot >= 0 && slot < 3) {
		return Coordinate(slot + (slot + 1), 1);
		
		//This is the first row of normal slots
	} else if (slot >= 3 && slot < 6) {
		return Coordinate(slot - 1, 4);

		//This is the second row of normal slots
	} else if (slot >= 6) {
		return Coordinate(slot - 4, 5);
	}
}


//Get a random value for a type, given the type it should be
int Game::get_random_value_by_type(std::string type, std::string compare_type) {
	//Get values for the armor, weapon, and magic 
	if (type == compare_type) {
		if (type == "Armor") {
			//Armor is from 1 to 10
			return ((rand() % 10) + 1);
		}
		else if (type == "Weapon") {
			//Weapon damage is from 1 to 20
			return ((rand() % 20) + 1);
		}
		else {
			//Magic damage is from 1 to 10 (though this has no current effect)
			return ((rand() % 10) + 1);
		}
	}
	
	//If the type is not the one we want, its value should be zero
	//(This is because a Weapon item will have an Equipment component,  
	//but it should not have armor or magic values
	return 0;
}


//Get a random item type (used in item generation)
std::string Game::get_random_item_type() {
	//kItemTypes represents the number of item types available in the gmae
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


//Generate a random name for an item based on the type of the item
std::string Game::generate_random_name(std::string type) {
	//There is only one healing item - so its name is set
	if (type == "Healing") {
		return "Healing Potion";
	}

	//Constant sizes for the prefixes, suffixes, and names for random selection
	const int prefix_size = 15;
	const int suffix_size = 6;
	const int name_size = 5;

	//A list of prefixes to randomly choose from
	std::string prefixes[prefix_size] = { "Aquatic", "Burning", "Shining", "Gleaming", "Crystalline",
			"Rough", "Smooth", "Oceanic", "Soaked", "Drenched",
			"Crabman's", "Scavenger's", "Explorer's", "Geoff's", "Carl's" };

	//A list of suffixes to randomly choose from
	std::string suffixes[suffix_size] = { "of the Ocean", "of the Lake", "of the River",
			"of the Sand", "of the Octopus", "of the Eel" };

	std::string prefix_string = "";
	std::string suffix_string = "";
	std::string name_string = "";

	int prefix = rand() % (2 * prefix_size);
	int suffix = rand() % (2 * suffix_size);

	//Represents the index of the name from name arrays located below
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
		//If for some reason the item is unimplemented, name it a random item bag (it will also display as such)
		//Note the Random Item Bag is non-functional and is a placeholder
		return "Random Item Bag";
	}
}

//Gets a random image name based on the item type
//(Technically only random if more than one image exists)
//The healing potion is set to be only one image so it is not random
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
		//The Random Item Bag is a placeholder
		return "random_item_bag";
	}
}


//See if an item can be added to a player's inventory and then attempt to add it
void Game::check_to_add_item() {

	//Do not attempt to add an item if the player's inventory is full
	if (player.is_inventory_full()) {
		return;
	}

	auto items = registry.view<Item>();
	auto locations = registry.view<Location>();

	//Search through the item entities
	for (auto entity : items) {

		//If they have a location and are NOT inventory items (which have relative locations), check the item's location
		if (registry.has<Location>(entity) && !registry.has<InventorySlot>(entity)) {
			auto &loc = locations.get(entity);
			auto &stats = items.get(entity);

			//If the location of the item is the same as the player, pick it up
			if (loc.current_tile == player.get_current_tile()) {
				//Give it an inventory slot component as part of the inventory, assigning it to the first empty slot
				registry.assign<InventorySlot>(entity, player.get_first_empty_slot(), stats.type);

				//Give it a relative location based on its inventory slot
				registry.replace<Location>(entity, player.coord_of_first_empty_slot());

				//Mark the slot it takes up as occupied
				player.occupy_slot(player.get_first_empty_slot());
			}
		}
	}
}


//Returns the proper modifier based on the player's inventory slot (0 - 2)
//0 is the Weapon modifier, 1 is the Armor modifier, and 2 is the Magic modifier
int Game::get_modifier(int slot) {
	//If it is not a number 0 - 2 do not continue
	if (slot > 2 || slot < 0) {
		return 0;
	}

	//Look at the relative coordinate of the slot
	Coordinate destination = get_coordinate_from_slot(slot);
	auto inventory_items = registry.view<InventorySlot>();
	auto locations = registry.view<Location>();
	auto equipment = registry.view<Equipment>();

	//Search through the items in the player's inventory
	for (auto item : inventory_items) {
		auto &loc = locations.get(item);

		//If the location of the item is in the slot return the proper modifier
		if (registry.has<Equipment>(item)) {
			auto &equip_stats = equipment.get(item);
			if (destination == loc.current_tile) {
				if (slot == 0) {
					//Weapon modifier
					return equip_stats.melee_modifier;
				} else if (slot == 1) {
					//Armor modifier
					return equip_stats.armor_strength;
				} else if (slot == 2) {
					//Magic modifier
					return equip_stats.magic_modifier;
				}
			}
		}
	}

	//If anything goes wrong, return the modifier as 0
	return 0;
}


//Attempt to spawn a boss
void Game::attempt_boss_spawn() {

	//Only attempt to spawn a boss if a player is on a boss spawn tile
	if (level.get_tile(player.get_current_tile()).get_type() == "boss_spawn") {
		boss_mode = true;
		//Replace the boss spawn tile with a floor tile
		level.set_tile(player.get_player_x(), player.get_player_y(), Tile(true, "no_spawn_floor"));

		//Wall off the entrances to the boss room and destroy any entities that were there
		//The parameters of the loops represent a square radius around the player
		for (int y = player.get_player_y() - 3; y < player.get_player_y() + 4; y++) {
			for (int x = player.get_player_x() - 3; x < player.get_player_x() + 4; x++) {

				//If there is a boss gateway tile in the radius, wall it off
				if (level.is_valid_coordinate(x, y) && level.get_tile(x, y).get_type() == "boss_gateway") {
					auto locations = registry.view<Location>();

					//If there is an entity on the gateway tile, destroy it (inventory items don't count)
					for (auto entity : locations) {
						auto &loc = locations.get(entity);
						if ((registry.has<Item>(entity) && !registry.has<InventorySlot>(entity)) || registry.has<Enemy>(entity) 
							&& loc.current_tile == Coordinate(x, y)) {
							registry.destroy(entity);
						}
					}

					//Change the gateway to a wall tile
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
			//The tiles representing the main portion of its body are impassable
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

		//For the Cuttlefish's action, update the current tendrils, then summon a new one
		if (name == boss_stats.name && name == "Cuttlefish") {
			update_arms_in_radius(boss_stats.start, 3, boss_stats.strength);
			spawn_tendril(level.get_random_passable_tile_in_radius(boss_stats.start, 3), 5);
		}
	}
	
}


//Part of the Cuttlefish boss's attack. Summons a tendril
void Game::spawn_tendril(Coordinate center, int distance) {
	int attempts = 0;
	//The current coordinate starts as the initial center coordinate
	Coordinate current = Coordinate(center.get_coordinate_x(), center.get_coordinate_y());

	//Spread in the direction of the player
	std::string direction = orient_towards_player(center);
	int x_modifier = 0;
	int y_modifier = 0;

	//Set the modifiers based on the calculated direction
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

	//For the passed distance, extend the tendril in the direction on valid tiles (boss room floor tiles)
	//Additionally, set the proper tile (vertical or horizontal) based on the direction
	for (int i = 0; i < distance; i++) {
		if (level.is_valid_passable_tile(current)) {
			if ((direction == "up" || direction == "down") && level.get_tile(current).get_type() == "no_spawn_floor" && level.get_tile(current).get_passability()) {
				level.set_tile(current.get_coordinate_x(), current.get_coordinate_y(), Tile(true, "arm_1_v"));
			}

			if ((direction == "left" || direction == "right") && level.get_tile(current).get_type() == "no_spawn_floor" && level.get_tile(current).get_passability()) {
				level.set_tile(current.get_coordinate_x(), current.get_coordinate_y(), Tile(true, "arm_1_h"));
			}

			//Adjust the current tile
			current = Coordinate(current.get_coordinate_x() + x_modifier, current.get_coordinate_y() + y_modifier);
			
		}
	}

	
}


//Part of the Cuttlefish boss's action. Tells a tendril what direction to face when it spawns
std::string Game::orient_towards_player(Coordinate center) {
	//Start with the y and x distance from the player, and ensure they are positive
	int y_distance = center.get_coordinate_y() - player.get_player_y();
	int x_distance = center.get_coordinate_x() - player.get_player_x();
	if (x_distance < 0) {
		x_distance *= -1;
	}
	if (y_distance < 0) {
		y_distance *= -1;
	}

	//Get the direction based on which distance is smaller, 
	//and the location of the center relative to the player
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

	//By default if the distance are equal move right
	if (x_distance == y_distance) {
		return "right";
	}
}


//Part of the Cuttlefish boss's action. Updates all of its tendrils
//On the 3rd turn a tendril exists, it will damage a player standing on it,
//as well as becoming impassable for a turn. It disappears after this third turn.
void Game::update_arms_in_radius(Coordinate center, int radius, int strength) {
	//Update the tendrils in the given radius around the center
	for (int y = center.get_coordinate_y() - radius; y <= center.get_coordinate_y() + radius; y++) {
		for (int x = center.get_coordinate_x() - radius; x <= center.get_coordinate_x() + radius; x++) {
			//If the coordinates are valid, continue
			if (level.is_valid_coordinate(x, y)) {
				//Change turn 1 arms to turn 2 arms
				if (level.get_tile(x, y).get_type() == "arm_1_v") {
					level.set_tile(x, y, Tile(true, "arm_2_v"));

				} else if (level.get_tile(x, y).get_type() == "arm_1_h") {
					level.set_tile(x, y, Tile(true, "arm_2_h"));

				} else if (level.get_tile(x, y).get_type() == "arm_2_v") {
					//Change turn 2 arms into turn 3 arms, and damage a player if necessary
					if (x == player.get_player_x() && y == player.get_player_y()) {
						attack_player(strength);
					}
					level.set_tile(x, y, Tile(false, "arm_3_v"));

				} else if (level.get_tile(x, y).get_type() == "arm_2_h") {
					//Change turn 2 arms into turn 3 arms, and damage a player if necessary
					if (x == player.get_player_x() && y == player.get_player_y()) {
						attack_player(strength);
					}
					level.set_tile(x, y, Tile(false, "arm_3_h"));

				} else if (level.get_tile(x, y).get_type() == "arm_3_v" || level.get_tile(x, y).get_type() == "arm_3_h") {
					//Change turn 3 arms into boss floor tiles again
					level.set_tile(x, y, Tile(true, "no_spawn_floor"));
				}
			}
		}
	}
}