#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	srand(time(NULL));

	std::cout << "Loading graphics...\n";
	//Loading tiles
	bridge_v.load("graphics/Tiles/bridge_vertical.png");
	bridge_h.load("graphics/Tiles/bridge_horizontal.png");
	water.load("graphics/Tiles/water.png");
	ceiling.load("graphics/Tiles/ceiling.png");
	floor.load("graphics/Tiles/floor.png"); 
	wall.load("graphics/Tiles/wall.png");
	boss_wall.load("graphics/Tiles/boss_wall.png");
	
	//Player graphics
	player_front.load("graphics/Sprites/Player/cedar_front.png");
	player_left.load("graphics/Sprites/Player/cedar_left.png");
	player_right.load("graphics/Sprites/Player/cedar_right.png");
	player_back.load("graphics/Sprites/Player/cedar_back.png");

	//UI graphics
	health_bar.load("graphics/UI/health_bar.png");
	health_strip.load("graphics/UI/health_slice.png");
	combat_tile.load("graphics/UI/combat_tile.png");
	enemy_health_bar.load("graphics/UI/enemy_health_bar.png");
	enemy_health_strip.load("graphics/UI/enemy_health_slice.png");

	//Enemy graphics
	crabman_front.load("graphics/Sprites/Crabman/crabman_front.png");
	crabman_left.load("graphics/Sprites/Crabman/crabman_left.png");
	crabman_right.load("graphics/Sprites/Crabman/crabman_right.png");
	crabman_back.load("graphics/Sprites/Crabman/crabman_back.png");
	octopus_front.load("graphics/Sprites/Octopus/octopus_front.png");
	octopus_left.load("graphics/Sprites/Octopus/octopus_left.png");
	octopus_right.load("graphics/Sprites/Octopus/octopus_right.png");
	octopus_back.load("graphics/Sprites/Octopus/octopus_back.png");
	eel_front.load("graphics/Sprites/Eel/eel_front.png");
	eel_left.load("graphics/Sprites/Eel/eel_left.png");
	eel_right.load("graphics/Sprites/Eel/eel_right.png");
	eel_back.load("graphics/Sprites/Eel/eel_back.png");

	
	//Item graphics
	dagger.load("graphics/Items/dagger.png");
	gladius.load("graphics/Items/gladius.png");
	health_potion.load("graphics/Items/health_potion.png");
	magic_orb_1.load("graphics/Items/magic_orb_1.png");
	magic_orb_2.load("graphics/Items/magic_orb_2.png");
	magic_orb_3.load("graphics/Items/magic_orb_3.png");
	random_item_bag.load("graphics/Items/random_item_bag.png");
	shield.load("graphics/Items/shield.png");



	level.load_room_presets();
	std::cout << "Creating the level...\n";
	level.instantiate_level();
	player = Player(level.get_start_tile(), 100, 10);
	create_entities();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (player.get_health() <= 0) {
		std::exit(0);
	}

	auto stats = registry.view<Enemy>();
	for (auto entity : stats) {
		auto &opponent = stats.get(entity);
		if (opponent.health <= 0) {
			registry.destroy(entity);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	int pixel_x = 0;
	int pixel_y = 0;

	auto location = registry.view<Location>();
	auto enemies = registry.view<Enemy>();

	int display_y = player.get_player_y() - 3;
	int display_x = player.get_player_x() - 3;


	//Tile display
	for (int y = display_y; y < display_y + kDisplaySize; y++) {
		for (int x = display_x; x < display_x + kDisplaySize; x++) {
			if ((x >= 0 && x < kSize) && (y >= 0 && y < kSize)) {
				ofImage image = get_image_from_type(level.get_tile(x, y).get_type());
				image.draw(pixel_x, pixel_y);
			}

			if (combat && is_combat_space(x, y)) {
				combat_tile.draw(pixel_x, pixel_y);
			}

			for (auto entity : location) {
				auto &loc = location.get(entity);
				if (loc.current_tile.get_coordinate_x() == x && loc.current_tile.get_coordinate_y() == y) {
					if (registry.has<Enemy>(entity)) {
						auto &stats = enemies.get(entity);
						ofImage enemy = get_image(stats.name, stats.direction);
						enemy.draw(pixel_x, pixel_y);
					}
				}
			}


			pixel_x += 128;
		}
		pixel_x = 0;
		pixel_y += 128;
	}

	


	//Player display
	//Player is always displayed in center of screen
	ofImage player_stance = get_image_from_direction(player.get_turn_direction());
	player_stance.draw(kDisplayCenter, kDisplayCenter);

	//UI Display

	pixel_x = 0;
	pixel_y = 0;
	//Enemy health display
	for (int y = display_y; y < display_y + kDisplaySize; y++) {
		for (int x = display_x; x < display_x + kDisplaySize; x++) {
			for (auto entity : location) {
				auto &loc = location.get(entity);
				if (loc.current_tile.get_coordinate_x() == x && loc.current_tile.get_coordinate_y() == y) {
					if (registry.has<Enemy>(entity)) {
						auto &stats = enemies.get(entity);

						if (stats.health != stats.total_health) {
							float enemy_health_portion = (float)((float)stats.health / (float)stats.total_health);
							int num_enemy_health_strips = enemy_health_portion * 15;
							for (int sub_x = pixel_x; sub_x < num_enemy_health_strips * 8 + pixel_x; sub_x += 8) {
								enemy_health_strip.draw(sub_x, pixel_y - 32);
							}

							enemy_health_bar.draw(pixel_x, pixel_y - 88);
						}
					}
				}
			}


			pixel_x += 128;
		}
		pixel_x = 0;
		pixel_y += 128;
	}

	//Calculates portion of health bar to show, then displays health overlayed by health bar
	float health_portion = (float)((float)player.get_health() / (float)player.get_total_health());
	int num_health_strips = health_portion * 29;
	for (int x = 28; x < 28 + (num_health_strips * 6); x += 6) {
		health_strip.draw(x, 31);
	}

	health_bar.draw(16, 0);
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//Set players current x and y as shorthand for reference
	int x = player.get_player_x();
	int y = player.get_player_y();

	//When the player moves, check if the direction is valid and then move the player
	//Then take an enemy action, since it is the enemies' turn
	if (key == 'w') {
		player.set_turn_direction("up");
		if (is_tile_unobstructed(x, y - 1) && !combat) {
			player.set_current_tile(x, y - 1);
			enemies_action();
		}
	} else if (key == 'a') {
		player.set_turn_direction("left");
		if (is_tile_unobstructed(x - 1, y) && !combat) {
			player.set_current_tile(x - 1, y);
			enemies_action();
		}
	} else if (key == 's') {
		player.set_turn_direction("down");
		if (is_tile_unobstructed(x, y + 1) && !combat) {
			player.set_current_tile(x, y + 1);
			enemies_action();
		}
	} else if (key == 'd') {
		player.set_turn_direction("right");
		if (is_tile_unobstructed(x + 1, y) && !combat) {
			player.set_current_tile(x + 1, y);
			enemies_action();
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//Right click cycles 'combat' mode which displays what tiles the player can attack from
	if (button == 2 && !combat) {
		combat = true;
	} else if (button == 2 && combat) {
		combat = false;
	}

	//Left click is the attack button
	//Checks to make sure the attack is within range and there is an enemy on the clicked tile
	if (button == 0 && is_combat_space(get_coordinate_from_pixel(x, y)) 
		&& is_enemy_at_position(get_coordinate_from_pixel(x,y))) {
		attack_enemy_at_tile(get_coordinate_from_pixel(x, y));
		enemies_action();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

ofImage ofApp::get_image_from_type(std::string type) {
	//Gets the ofImage corresponding to the tile type
	if (type == "bridge_v") {
		return bridge_v;
	} else if (type == "bridge_h") {
		return bridge_h;
	} else if (type == "water") {
		return water;
	} else if (type == "floor" || type == "boss_spawn"
		|| type == "boss_gateway" || type == "no_spawn_floor") {
		return floor;
	} else if (type == "wall") {
		return wall;
	} else if (type == "boss_wall") {
		return boss_wall;
	} else {
		return ceiling;
	}
}

ofImage ofApp::get_image_from_direction(std::string direction) {
	if (direction == "up") {
		return player_back;
	} else if (direction == "left") {
		return player_left;
	} else if (direction == "right") {
		return player_right;
	} else {
		return player_front;
	}
}

//Gets the image for an enemy based on its name and direction
ofImage ofApp::get_image(std::string name, std::string direction) {
	if (name == "Crabman") {
		if (direction == "up") {
			return crabman_back;
		} else if (direction == "left") {
			return crabman_left;
		} else if (direction == "right") {
			return crabman_right;
		} else {
			return crabman_front;
		}
	} else if (name == "Octopus") {
		if (direction == "up") {
			return octopus_back;
		} else if (direction == "left") {
			return octopus_left;
		} else if (direction == "right") {
			return octopus_right;
		} else {
			return octopus_front;
		}
	} else {
		if (direction == "up") {
			return eel_back;
		} else if (direction == "left") {
			return eel_left;
		} else if (direction == "right") {
			return eel_right;
		} else {
			return eel_front;
		}
	}
}

//Create the entities
void ofApp::create_entities() {
	//The range of enemies to spawn in the level
	int num_entities = (rand() % 10) + 5;
	
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
bool ofApp::is_enemy_at_position(int x, int y) {
	auto locations = registry.view<Location>();
	for (auto entity : locations) {
		if (registry.has<Enemy>(entity)) {
			auto &loc = locations.get(entity);
			if (loc.current_tile.get_coordinate_x() == x && loc.current_tile.get_coordinate_y() == y) {
				return true;
			}
		}
	}

	return false;
}

//Checks if an enemy is at the current point
bool ofApp::is_enemy_at_position(Coordinate coordinate) {
	return is_enemy_at_position(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}


//Checks if a space is within the player's attack range
bool ofApp::is_combat_space(int x, int y) {
	//Shorthand for the player's x and y
	int player_x = player.get_player_x();
	int player_y = player.get_player_y();

	//If the tile is immediately above, below, left, or right 
	if (x == player_x && (y == player_y + 1 || y == player_y - 1)) {
		return true;
	} else if (y == player_y && (x == player_x + 1 || x == player_x - 1)) {
		return true;
	}

	return false;
}

bool ofApp::is_combat_space(Coordinate coordinate) {
	return is_combat_space(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}

void ofApp::enemies_action() {
	Coordinate destination;
	auto locations = registry.view<Location>();
	auto stats = registry.view<Enemy>();
	for (auto entity : locations) {
		if (registry.has<Enemy>(entity)) {
			auto &loc = locations.get(entity);
			auto &dir = stats.get(entity);
			destination = loc.current_tile;
			if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) == 1) {
				attack_player(dir.strength);
			} else if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) <= 3) {
				destination = move_enemy_towards_player(loc.current_tile.get_coordinate_x(), loc.current_tile.get_coordinate_y());
			} else if (get_euclidean_distance(loc.current_tile, player.get_current_tile()) > 3) {
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
}

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

std::string ofApp::determine_direction_relatively(Coordinate start, Coordinate end) {
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


Coordinate ofApp::get_coordinate_from_pixel(int pixel_x, int pixel_y) {
	int x = pixel_x / 128;
	int y = pixel_y / 128;
	x += (player.get_player_x() - 3);
	y += (player.get_player_y() - 3);

	return Coordinate(x, y);
}

void ofApp::attack_enemy_at_tile(Coordinate target) {
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




Coordinate ofApp::move_enemy_towards_player(int current_x, int current_y) {
	int x = current_x;
	int y = current_y;
	int distance = get_euclidean_distance(Coordinate(x, y), player.get_current_tile());
	if (is_distance_reduced(distance, Coordinate(x + 1, y), player.get_current_tile())) {
		return Coordinate(x + 1, y);
	} else if (is_distance_reduced(distance, Coordinate(x - 1, y), player.get_current_tile())) {
		return Coordinate(x - 1, y);
	} else if (is_distance_reduced(distance, Coordinate(x, y + 1), player.get_current_tile())) {
		return Coordinate(x, y + 1);
	} else if (is_distance_reduced(distance, Coordinate(x, y - 1), player.get_current_tile())) {
		return Coordinate(x, y - 1);
	}
	else {
		return Coordinate(x, y);
	}
	
}



bool ofApp::is_distance_reduced(int initial_d, Coordinate new_start, Coordinate end) {
	int final_distance = get_euclidean_distance(new_start, end);
	if (final_distance < initial_d && level.is_valid_passable_tile(new_start) 
		&& !is_enemy_at_position(new_start) 
		&& (new_start.get_coordinate_x() != player.get_player_x() || new_start.get_coordinate_y() != player.get_player_y())) {
		return true;
	}

	return false;
}

void ofApp::attack_player(int damage) {
	player.change_health(damage *  -1);
}


//Shorthand utility function for checking during player movement
bool ofApp::is_tile_unobstructed(int x, int y) {
	if (level.is_valid_coordinate(x, y) 
		&& level.get_tile(x, y).get_passability() && !is_enemy_at_position(x, y)) {
		return true;
	}

	return false;
}

bool ofApp::is_tile_unobstructed(Coordinate coordinate) {
	return is_tile_unobstructed(coordinate.get_coordinate_x(), coordinate.get_coordinate_y());
}