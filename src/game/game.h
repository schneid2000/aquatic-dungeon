#pragma once

#include "../map/level.h"
#include "../player/player.h"
#include "../entities/entity.h"
#include <entt/entt.hpp>

static const int kBossTypes = 1; //Cuttlefish
static const int kEnemyTypes = 3; //Crabman, Eel, Octopus
static const int kItemTypes = 5; //Armor items, Magic items, Weapons, Healing items, Normal items
static const int kMinEntities = 6; //The actual value this represents will be 1 less
static const int kMaxAdditionalEntities = 10; //The actual value this represents will be 1 less

static const int kWeaponImages = 2; 
static const int kArmorImages = 1;
static const int kMagicImages = 3;

class Game {
private:
	//The game level, which contains the map
	Level level;

	//The player
	Player player;

	//The EnTT registry, which keeps track of enemies, items, and the player's inventory
	entt::DefaultRegistry registry;

	//The mode
	//"default", "inventory", "combat"
	std::string mode;

	//The player needs to be able to access inventory and combat mode while in a boss fight
	//So boss mode is considered a separate variable, true if on, false if off
	bool boss_mode;




public:
	//Creates the a default style game
	Game();

	//Getters
	Level& get_level();
	Player& get_player();
	entt::DefaultRegistry& get_registry();
	std::string get_mode();
	bool is_boss_mode();

	//Returns true if in combat mode
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
	
	//Takes a turn for the boss based on the boss type
	void boss_action(std::string name);

	void spread_from_center(Coordinate center, int distance);
	std::string orient_towards_player(Coordinate center);
	void update_arms_in_radius(Coordinate center, int radius, int strength);

	//Choose a random position (adjacent or no movement) for the enemy to move to
	Coordinate move_enemy_randomly(int x, int y);
	Coordinate move_enemy_randomly(Coordinate coordinate);

	//Gets the relative direction (to know which way to display an enemy)
	std::string determine_direction_relatively(Coordinate start, Coordinate end);

	//Get the coordinate on the map from where a user clicked
	Coordinate get_coordinate_from_pixel(int pixel_x, int pixel_y);

	//Get the relative coordinate based on the display from where a user clicked
	Coordinate get_relative_coordinate_from_pixel(int pixel_x, int pixel_y);


	//A player attacks an enemy at a given map tile
	void attack_enemy_at_tile(Coordinate target);
	
	//An enemy attacks a player with a given amount of damage
	void attack_player(int damage);

	//An enemy attempts to move closer to a player
	Coordinate move_enemy_towards_player(int current_x, int current_y);

	//Returns true if the calculated distance is smaller than the initial distance and the path is unobstructed
	bool is_distance_reduced(int initial_d, Coordinate new_start, Coordinate end);

	
	//Checks if a particular tile on the map has no obstacles
	bool is_tile_unobstructed(int x, int y);
	bool is_tile_unobstructed(Coordinate coordinate);
	
	//Returns the proper inventory slot (0 - 8) based on the coordinate
	//Returns -1 if it receives an invalid coordinate
	int get_slot_from_relative_coordinate(Coordinate coordinate);

	//Gets the relative coordinate of an inventory slot 0 - 8
	Coordinate get_coordinate_from_slot(int slot);

	//Gets a random value for the modifiers for weapons, armor, and magic items
	int get_random_value_by_type(std::string type, std::string compare_type);

	//Gets a random type for an item when it is created (out of Weapon, Armor, Magic, Healing)
	std::string get_random_item_type();

	//Gets a random name for an item based on its type (Weapon, Armor, Magic get random names, Healing is set)
	std::string generate_random_name(std::string type);

	//Gets a random image name for an item based on its type
	std::string get_random_image_name(std::string type);

	//See if an item can be added to the player's inventory, then attempt to add it
	void check_to_add_item();

	//Retrieve the proper modifier based on the item in a slot (0 - 2)
	//Returns 0 if there is no item in the slot
	//0 is the melee modifier, 1 is the armor modifier, and 2 is the magic modifier
	int get_modifier(int slot);


	//Attempt to spawn a boss, starting a boss fight and setting the game to boss mode
	void attempt_boss_spawn();
	


};