#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	bridge.load("graphics/Tiles/bridge_vertical.png");
	water.load("graphics/Tiles/water.png");
	crabman_front.load("graphics/Sprites/Crabman/crabman_front.png");
	ceiling.load("graphics/Tiles/ceiling.png");
	floor.load("graphics/Tiles/floor.png"); 
	level.setup_start_tiles();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	int pixel_x = 0;
	int pixel_y = 0;
	std::cout << level.get_start_display_tile().get_coordinate_x() << ", " << level.get_start_display_tile().get_coordinate_y() << "\n";

	for (int y = level.get_start_display_tile().get_coordinate_y(); y < level.get_start_display_tile().get_coordinate_y() + kDisplaySize; y++) {
		for (int x = level.get_start_display_tile().get_coordinate_x(); x < level.get_start_display_tile().get_coordinate_x() + kDisplaySize; x++) {
			if ((x >= 0 && x < kSize) && (y >= 0 && y < kSize)) {
				ofImage image = get_image_from_type(level.get_tile(y, x).get_type());
				image.draw(pixel_x, pixel_y);
				
			}
			pixel_x += 128;
		}
		pixel_x = 0;
		pixel_y += 128;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
		return bridge;
	} else if (type == "water") {
		return water;
	} else if (type == "floor") {
		return floor;
	} else {
		return ceiling;
	}
}
