#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	srand(time(NULL));
	std::cout << "Loading graphics...\n";
	bridge_v.load("graphics/Tiles/bridge_vertical.png");
	bridge_h.load("graphics/Tiles/bridge_horizontal.png");
	water.load("graphics/Tiles/water.png");
	crabman_front.load("graphics/Sprites/Crabman/crabman_front.png");
	ceiling.load("graphics/Tiles/ceiling.png");
	floor.load("graphics/Tiles/floor.png"); 
	wall.load("graphics/Tiles/wall.png");
	player_front.load("graphics/Sprites/Player/cedar_front.png");
	player_left.load("graphics/Sprites/Player/cedar_left.png");
	player_right.load("graphics/Sprites/Player/cedar_right.png");
	player_back.load("graphics/Sprites/Player/cedar_back.png");
	level.load_room_presets();
	std::cout << "Creating the level...\n";
	level.instantiate_level();
	level.setup_start_tiles();
	player = Player(level.get_start_tile());
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	int pixel_x = 0;
	int pixel_y = 0;
	//std::cout << level.get_start_display_tile().get_coordinate_x() << ", " << level.get_start_display_tile().get_coordinate_y() << "\n";

	for (int y = player.get_current_tile().get_coordinate_y() - 3; y < player.get_current_tile().get_coordinate_y() - 3 + kDisplaySize; y++) {
		for (int x = player.get_current_tile().get_coordinate_x() - 3; x < player.get_current_tile().get_coordinate_x() - 3 + kDisplaySize; x++) {
			if ((x >= 0 && x < kSize) && (y >= 0 && y < kSize)) {
				ofImage image = get_image_from_type(level.get_tile(x, y).get_type());
				image.draw(pixel_x, pixel_y);
				if (x == player.get_player_x() && y == player.get_player_y()) {
					ofImage player_stance = get_image_from_direction(player.get_turn_direction());
					player_stance.draw(pixel_x, pixel_y);
				}
				
			}
			pixel_x += 128;
		}
		pixel_x = 0;
		pixel_y += 128;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	int x = player.get_player_x();
	int y = player.get_player_y();
	if (key == 'w') {
		player.set_turn_direction("up");
		if (level.is_valid_coordinate(x, y - 1) && level.get_tile(x, y - 1).get_passability()) {
			player.set_current_tile(x, y - 1);
		}
	} else if (key == 'a') {
		player.set_turn_direction("left");
		if (level.is_valid_coordinate(x - 1, y) && level.get_tile(x - 1, y).get_passability()) {
			player.set_current_tile(x - 1, y);
		}
	} else if (key == 's') {
		player.set_turn_direction("down");
		if (level.is_valid_coordinate(x, y + 1) && level.get_tile(x, y + 1).get_passability()) {
			player.set_current_tile(x, y + 1);
		}
	} else if (key == 'd') {
		player.set_turn_direction("right");
		if (level.is_valid_coordinate(x + 1, y) && level.get_tile(x + 1, y).get_passability()) {
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
	} else if (type == "floor") {
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
