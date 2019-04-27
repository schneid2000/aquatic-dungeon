#include "game.h"

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


		if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) > 1 && (loc.current_tile.get_coordinate_x() != destination.get_coordinate_x()) || loc.current_tile.get_coordinate_y() != destination.get_coordinate_y()) {
			dir.direction = determine_direction_relatively(loc.current_tile, destination);
		}
		else {
			dir.direction = determine_direction_relatively(loc.current_tile, player.get_current_tile());
		}
		loc.current_tile = destination;
	}
}


//Moves an enemy randomly
Coordinate ofApp::move_enemy_randomly(int current_x, int current_y) {
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