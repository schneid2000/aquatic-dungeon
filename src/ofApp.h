#pragma once

#include "ofMain.h"
#include "game/game.h"
#include "map/level.h"
#include "player/player.h"
#include "entities/entity.h"
#include <iostream>
#include <entt/entt.hpp>

static const int kDisplaySize = 7;
static const int kCardinalDirections = 4;
static const int kDisplayCenter = 384;
static const int kTileSize = 128;



class ofApp : public ofBaseApp{
	ofImage bridge_v;
	ofImage bridge_h;
	ofImage wall;
	ofImage water;
	ofImage ceiling;
	ofImage floor;
	ofImage boss_wall;
	ofImage boss_spawn;
	ofImage player_front;
	ofImage player_left;
	ofImage player_right;
	ofImage player_back;
	ofImage health_bar;
	ofImage health_strip;
	ofImage combat_tile;
	ofImage enemy_health_bar;
	ofImage enemy_health_strip;
	ofImage inventory_slot;
	ofImage selected_inventory_slot;
	ofImage weapon_slot;
	ofImage magic_slot;
	ofImage armor_slot;
	ofImage boss_health_bar;
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
	ofImage cuttlefish;
	ofImage dagger;
	ofImage gladius;
	ofImage health_potion;
	ofImage magic_orb_1;
	ofImage magic_orb_2;
	ofImage magic_orb_3;
	ofImage random_item_bag;
	ofImage shield;
	ofTrueTypeFont press_start_2p;
	Game game;
	

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
		ofImage get_image(std::string name, std::string direction);

		//Gets the image for an item type
		ofImage get_image_from_item(std::string image);

		//Gets the image for a boss type
		ofImage get_image_from_boss(std::string name);

		
};
