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
	inventory_slot.load("graphics/UI/inventory_slot.png");
	weapon_slot.load("graphics/UI/inventory_slot_weapon.png");
	armor_slot.load("graphics/UI/inventory_slot_armor.png");
	magic_slot.load("graphics/UI/inventory_slot_magic.png");



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

	//Font 
	press_start_2p.load("graphics/Fonts/PressStart2P.ttf", 16);

	std::cout << "Creating the level...\n";
}

//--------------------------------------------------------------
void ofApp::update(){
	if (game.get_player().get_health() <= 0) {
		std::exit(0);
	}

	auto stats = game.get_registry().view<Enemy>();
	for (auto entity : stats) {
		auto &opponent = stats.get(entity);
		if (opponent.health <= 0) {
			game.get_registry().remove<Enemy>(entity);
			std::string type = game.get_random_item_type();
			game.get_registry().assign<Item>(entity, game.generate_random_name(type), type, game.get_random_image_name(type));
		}
	}

	game.check_to_add_item();
}

//--------------------------------------------------------------
void ofApp::draw(){
	int pixel_x = 0;
	int pixel_y = 0;

	auto location = game.get_registry().view<Location>();
	auto enemies = game.get_registry().view<Enemy>();
	auto items = game.get_registry().view<Item>();

	int display_y = game.get_player().get_player_y() - 3;
	int display_x = game.get_player().get_player_x() - 3;


	//Tile display
	for (int y = display_y; y < display_y + kDisplaySize; y++) {
		for (int x = display_x; x < display_x + kDisplaySize; x++) {
			if ((x >= 0 && x < kSize) && (y >= 0 && y < kSize)) {
				ofImage image = get_image_from_type(game.get_level().get_tile(x, y).get_type());
				image.draw(pixel_x, pixel_y);
			}

			if (game.in_combat_mode() && game.is_combat_space(x, y)) {
				combat_tile.draw(pixel_x, pixel_y);
			}

			for (auto entity : location) {
				auto &loc = location.get(entity);
				if (loc.current_tile.get_coordinate_x() == x && loc.current_tile.get_coordinate_y() == y) {
					if (game.get_registry().has<Item>(entity) && !game.get_registry().has<InventorySlot>(entity)) {
						auto &info = items.get(entity);
						ofImage item_image = get_image_from_item(info.image);
						item_image.draw(pixel_x, pixel_y);
						if (loc.current_tile.get_coordinate_x() == game.get_player().get_player_x() && loc.current_tile.get_coordinate_y() == game.get_player().get_player_y()) {
							press_start_2p.drawString(info.name, 32, 128);
						}
					}


					if (game.get_registry().has<Enemy>(entity)) {
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
	ofImage player_stance = get_image_from_direction(game.get_player().get_turn_direction());
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
					if (game.get_registry().has<Enemy>(entity)) {
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

	if (game.get_mode() == "default") {
		//Calculates portion of health bar to show, then displays health overlayed by health bar
		float health_portion = (float)((float)game.get_player().get_health() / (float)game.get_player().get_total_health());
		int num_health_strips = health_portion * 29;
		for (int x = 28; x < 28 + (num_health_strips * 6); x += 6) {
			health_strip.draw(x, 31);
		}

		health_bar.draw(16, 0);
	}

	if (game.get_mode() == "inventory") {
		weapon_slot.draw(128, 128);
		armor_slot.draw(384, 128);
		magic_slot.draw(640, 128);
		for (int y = 512; y < 768; y += 128) {
			for (int x = 256; x < 640; x += 128) {
				inventory_slot.draw(x, y);
			}
		}

		auto locations = game.get_registry().view<Location>();
		auto inventory_items = game.get_registry().view<InventorySlot>();
		auto item_stats = game.get_registry().view<Item>();
		for (auto item : inventory_items) {
			auto &loc = locations.get(item);
			auto &stats = item_stats.get(item);
			ofImage image = get_image_from_item(stats.image);
			image.draw(128 * loc.current_tile.get_coordinate_x(), 128 * loc.current_tile.get_coordinate_y());
		}
	}
	
	
	



	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//Set players current x and y as shorthand for reference
	int x = game.get_player().get_player_x();
	int y = game.get_player().get_player_y();

	//Entering and exiting inventory mode
	if (key == 'e' && game.get_mode() == "default") {
		game.set_mode("inventory");
	} else if (key == 'e' && game.get_mode() == "inventory") {
		game.set_mode("default");
	}



	//When the player moves, check if the direction is valid and then move the player
	//Then take an enemy action, since it is the enemies' turn
	if (game.get_mode() != "inventory") {
		if (key == 'w') {
			game.get_player().set_turn_direction("up");
			if (game.is_tile_unobstructed(x, y - 1) && !game.in_combat_mode()) {
				game.get_player().set_current_tile(x, y - 1);
				game.enemies_action();
			}
		}
		else if (key == 'a') {
			game.get_player().set_turn_direction("left");
			if (game.is_tile_unobstructed(x - 1, y) && !game.in_combat_mode()) {
				game.get_player().set_current_tile(x - 1, y);
				game.enemies_action();
			}
		}
		else if (key == 's') {
			game.get_player().set_turn_direction("down");
			if (game.is_tile_unobstructed(x, y + 1) && !game.in_combat_mode()) {
				game.get_player().set_current_tile(x, y + 1);
				game.enemies_action();
			}
		}
		else if (key == 'd') {
			game.get_player().set_turn_direction("right");
			if (game.is_tile_unobstructed(x + 1, y) && !game.in_combat_mode()) {
				game.get_player().set_current_tile(x + 1, y);
				game.enemies_action();
			}
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
	if (button == 2 && !game.in_combat_mode()) {
		game.set_mode("combat");
	} else if (button == 2 && game.in_combat_mode()) {
		game.set_mode("default");
	}

	//Left click is the attack button
	//Checks to make sure the attack is within range and there is an enemy on the clicked tile
	if (button == 0 && game.is_combat_space(game.get_coordinate_from_pixel(x, y)) 
		&& game.is_enemy_at_position(game.get_coordinate_from_pixel(x,y))) {
		game.attack_enemy_at_tile(game.get_coordinate_from_pixel(x, y));
		game.enemies_action();
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

ofImage ofApp::get_image_from_item(std::string image) {
	if (image == "dagger") {
		return dagger;
	}
	else if (image == "sword") {
		return gladius;
	}
	else if (image == "shield") {
		return shield;
	}
	else if (image == "orb_1") {
		return magic_orb_1;
	}
	else if (image == "orb_2") {
		return magic_orb_2;
	}
	else if (image == "orb_3") {
		return magic_orb_3;
	}
	else {
		return random_item_bag;
	}
}