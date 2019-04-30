#pragma once

#include "../map/level.h"
#include "../player/player.h"
#include "../entities/entity.h"
#include <entt/entt.hpp>

static const int kEnemyTypes = 3;
static const int kItemTypes = 5; //Armor items, Magic items, Weapons, Healing items, Normal items
static const int kMinEntities = 6; //The actual value this represents will be 1 less
static const int kMaxAdditionalEntities = 10; //The actual value this represents will be 1 less

static const int kWeaponImages = 2;
static const int kArmorImages = 1;
static const int kMagicImages = 3;

class Game {
private:
	Level level;
	Player player;
	entt::DefaultRegistry registry;
	std::string mode;




public:
	//Creates the a default style game
	Game();

	//Getters
	Level& get_level();
	Player& get_player();
	entt::DefaultRegistry& get_registry();
	std::string get_mode();
	bool in_combat_mode();

	//Setters
	void set_mode(std::string new_mode);

	//Creates enemies and puts them in the level
	void create_entities();

	//Checks the map index for an enemy
	bool is_enemy_at_position(int x, int y);
	bool is_enemy_at_position(Coordinate coordinate);

	//Checks the space to see if the player can attack there
	bool is_combat_space(int x, int y);
	bool is_combat_space(Coordinate coordinate);

	//Takes a 'turn' for all enemies
	void enemies_action();

	//Choose a random position (adjacent or no movement) for the enemy to move to
	Coordinate move_enemy_randomly(int x, int y);
	Coordinate move_enemy_randomly(Coordinate coordinate);

	std::string determine_direction_relatively(Coordinate start, Coordinate end);


	Coordinate get_coordinate_from_pixel(int pixel_x, int pixel_y);
	Coordinate get_relative_coordinate_from_pixel(int pixel_x, int pixel_y);

	void attack_enemy_at_tile(Coordinate target);

	Coordinate move_enemy_towards_player(int current_x, int current_y);
	bool is_distance_reduced(int initial_d, Coordinate new_start, Coordinate end);

	void attack_player(int damage);

	bool is_tile_unobstructed(int x, int y);
	bool is_tile_unobstructed(Coordinate coordinate);
	

	int get_slot_from_relative_coordinate(Coordinate coordinate);
	Coordinate get_coordinate_from_slot(int slot);
	int get_random_value_by_type(std::string type, std::string compare_type);
	std::string get_random_item_type();
	std::string generate_random_name(std::string type);
	std::string get_random_image_name(std::string type);

	void check_to_add_item();

	int get_modifier(int slot);

};