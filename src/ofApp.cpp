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
	
	//Player graphics
	player_front.load("graphics/Sprites/Player/cedar_front.png");
	player_left.load("graphics/Sprites/Player/cedar_left.png");
	player_right.load("graphics/Sprites/Player/cedar_right.png");
	player_back.load("graphics/Sprites/Player/cedar_back.png");

	//UI graphics
	health_bar.load("graphics/UI/health_bar.png");
	health_strip.load("graphics/UI/health_slice.png");

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

	level.load_room_presets();
	std::cout << "Creating the level...\n";
	level.instantiate_level();
	player = Player(level.get_start_tile(), 100);
	create_entities();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	int pixel_x = 0;
	int pixel_y = 0;

	auto location = registry.view<Location>();
	auto enemies = registry.view<Enemy>();

	//Tile display
	for (int y = player.get_current_tile().get_coordinate_y() - 3; y < player.get_current_tile().get_coordinate_y() - 3 + kDisplaySize; y++) {
		for (int x = player.get_current_tile().get_coordinate_x() - 3; x < player.get_current_tile().get_coordinate_x() - 3 + kDisplaySize; x++) {
			if ((x >= 0 && x < kSize) && (y >= 0 && y < kSize)) {
				ofImage image = get_image_from_type(level.get_tile(x, y).get_type());
				image.draw(pixel_x, pixel_y);
			}

			for (auto entity : location) {
				auto &loc = location.get(entity);
				if (loc.current_tile.get_coordinate_x() == x && loc.current_tile.get_coordinate_y() == y) {
					if (registry.has<Enemy>(entity)) {
						auto &stats = enemies.get(entity);
						ofImage enemy = get_image_from_name(stats.name);
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
	int x = player.get_player_x();
	int y = player.get_player_y();
	if (key == 'w') {
		player.set_turn_direction("up");
		if (level.is_valid_coordinate(x, y - 1) && level.get_tile(x, y - 1).get_passability() && !is_enemy_at_position(x, y - 1)) {
			player.set_current_tile(x, y - 1);
		}
	} else if (key == 'a') {
		player.set_turn_direction("left");
		if (level.is_valid_coordinate(x - 1, y) && level.get_tile(x - 1, y).get_passability() && !is_enemy_at_position(x - 1, y)) {
			player.set_current_tile(x - 1, y);
		}
	} else if (key == 's') {
		player.set_turn_direction("down");
		if (level.is_valid_coordinate(x, y + 1) && level.get_tile(x, y + 1).get_passability() && !is_enemy_at_position(x, y + 1)) {
			player.set_current_tile(x, y + 1);
		}
	} else if (key == 'd') {
		player.set_turn_direction("right");
		if (level.is_valid_coordinate(x + 1, y) && level.get_tile(x + 1, y).get_passability() && !is_enemy_at_position(x + 1, y)) {
			player.set_current_tile(x + 1, y);
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

ofImage ofApp::get_image_from_type(std::string type) {
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

ofImage ofApp::get_image_from_name(std::string name) {
	if (name == "Crabman") {
		return crabman_front;
	} else if (name == "Octopus") {
		return octopus_front;
	} else {
		return eel_front;
	}
}

void ofApp::create_entities() {
	int num_entities = (rand() % 10) + 5;
	for (int i = 0; i < num_entities; i++) {
		auto entity = registry.create();
		registry.assign<Location>(entity, level.get_random_passable_tile());
		int type = rand() % kEnemyTypes;
		if (type == 0) {
			registry.assign<Enemy>(entity, 100, 20, "Crabman");
		} else if (type == 1) {
			registry.assign<Enemy>(entity, 40, 10, "Octopus");
		} else if (type == 2) {
			registry.assign<Enemy>(entity, 20, 10, "Eel");
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
