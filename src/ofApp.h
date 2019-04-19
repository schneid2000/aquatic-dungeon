#pragma once

#include "ofMain.h"
#include "map/level.h"
#include "player/player.h"
#include "entities/entity.h"
#include <iostream>
#include <entt/entt.hpp>

static const int kDisplaySize = 7;

class ofApp : public ofBaseApp{
	ofImage bridge_v;
	ofImage bridge_h;
	ofImage wall;
	ofImage water;
	ofImage ceiling;
	ofImage floor;
	ofImage crabman_front;
	ofImage player_front;
	ofImage player_left;
	ofImage player_right;
	ofImage player_back;
	ofImage health_bar;
	ofImage health_strip;
	Level level = Level(false);
	Player player = Player();

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//Gets the image from a tile's type
		ofImage get_image_from_type(std::string type);

		//Gets the image to use based on what direction the player turns in
		ofImage get_image_from_direction(std::string direction);

};
