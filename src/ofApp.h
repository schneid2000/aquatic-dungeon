#pragma once

#include "ofMain.h"
#include "map/level.h"
#include "player/player.h"
#include "entities/entity.h"
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
	ofImage boss_wall;
	ofImage player_front;
	ofImage player_left;
	ofImage player_right;
	ofImage player_back;
	ofImage health_bar;
	ofImage health_strip;
	ofImage combat_tile;
	ofImage enemy_health_bar;
	ofImage enemy_health_strip;
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
	ofImage dagger;
	ofImage gladius;
	ofImage health_potion;
	ofImage magic_orb_1;
	ofImage magic_orb_2;
	ofImage magic_orb_3;
	ofImage random_item_bag;
	ofImage shield;
	Level level = Level(false);
	Player player = Player();
	entt::DefaultRegistry registry;
	bool combat = false;
	

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

		//Add enemies to the level
		void create_entities();

		//Checks if there is an enemy at the desired position
		bool is_enemy_at_position(int x, int y);
		bool is_enemy_at_position(Coordinate coordinate);

		//Determines if this is a combat space
		bool is_combat_space(int x, int y);
		bool is_combat_space(Coordinate coordinate);

		//Enemies take their action
		void enemies_action();
		Coordinate move_enemy_randomly(int current_x, int current_y);
		//This function is intended for one-step movement
		std::string determine_direction_relatively(Coordinate start, Coordinate end);

		Coordinate get_coordinate_from_pixel(int pixel_x, int pixel_y);

		void attack_enemy_at_tile(Coordinate target);

		Coordinate move_enemy_towards_player(int current_x, int current_y);
		bool is_distance_reduced(int initial_d, Coordinate new_start, Coordinate end);
		void attack_player(int damage);

		bool is_tile_unobstructed(int x, int y);
		bool is_tile_unobstructed(Coordinate coordinate);
};
