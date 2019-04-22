#pragma once

#include "ofMain.h"
#include "map/level.h"
#include "player/player.h"
#include "entities/entity.cpp"
#include <iostream>
#include <entt/entt.hpp>

static const int kDisplaySize = 7;
static const int kEnemyTypes = 3;
static const int kDisplayCenter = 384;

class ofApp : public ofBaseApp{
	ofImage bridge_v;
	ofImage bridge_h;
	ofImage wall;
	ofImage water;
	ofImage ceiling;
	ofImage floor;
	ofImage player_front;
	ofImage player_left;
	ofImage player_right;
	ofImage player_back;
	ofImage health_bar;
	ofImage health_strip;
	ofImage crabman_front;
	ofImage crabman_left;
	ofImage crabman_right;
	ofImage crabman_back;
	ofImage octopus_front;
	ofImage octopus_left;
	ofImage octopus_right;
	ofImage octopus_back;
	ofImage eel_front;
	ofImage eel_left;
	ofImage eel_right;
	ofImage eel_back;
	Level level = Level(false);
	Player player = Player();
	entt::DefaultRegistry registry;
	

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

		//Gets the image for an enemy type
		ofImage get_image_from_name(std::string name);

		//Add enemies to the level
		void create_entities();

		//Checks if there is an enemy at the desired position
		bool is_enemy_at_position(int x, int y);
		bool is_enemy_at_position(Coordinate coordinate);

};
