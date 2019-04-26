# Development Process

## Beginning with EntityX

- Install Process
	-I needed to get CMake, which I have not used until now, for this library.
	-To run it with MINGW I also need the Boost library.
	-I successfully installed the boost library, as well as the make components of MinGW and MSYS
	-Using some help from StackOverflow I may have built the library: [source](https://stackoverflow.com/questions/11813844/building-hello-world-at-the-command-line-using-cmake-and-visual-studio-express-2)
	-Unfortunately I could not get the EntityX library working, most likely due to linking problems.

- Dependencies
	-Running the example code given with the EntityX library requried SFML, another graphics library.
	-I got SFML's example code working properly
	-In the process I understood more about the basic process of adding include directories and libaries to a project (This was important for using EnTT)

## Beginning with EnTT

- Install Process
	-I found EnTT, a superior library to EntityX (more or less as self-described in the github)
	-It is a header-only library, which was much more convenient to attempt to use
	-Got it working and got the example code to compile in Visual Studio




## April 13, 2019

- Map Mechanics
	-I needed to set up a lot of the basics in order to get anything visual working.
	-I prepared level and tile classes, to represent the floor layout and each square of that layout respectively.
	-I created a Coordinate class, so I can pass x and y coordinates of this map simultaneously.
	
- Obstacles
	-I created a constant for the size of the floor, and then forgot to use it to set the size of the floor
	-(Needless to say that fixed a lot of errors I was having)
	-Here and there I forgot an important setter
	-Showing the room in ofApp, I messed up adjusting where a tile should be drawn several times
	-Additionally, I miscalculated where it should be drawn (so only a 7x7 square was drawn instead of the 30x30)

- Progress
	-The map can now be viewed
	-A random possible spawn point is chosen and the surrounding area is shown
	-If a square is not part of the map, no tile is shown




## Week 1

-Map Mechanics
	-I completed setting up the Level class and its components; Coordinate, Room, and Tile.
	-I overrode the input stream for the room class, so I can input the rooms as presets from a text file.
	-I fixed some developer-friendly issues, like functions that took y, then x values, when x, y values would be more clear.

- Level Class Summary
	-The Level class contains most of the information about the environment the player moves around in.
	-I removed some unused code from the class.
	-I added several generation methods to create a random level:
		1. I created a method that randomly adds a room based on the presets from the presets file
		2. I created a method that generates a hallway in a random direction, starting at an impassable space.
		3. I added boss rooms, which are tagged differently in the room preset file.
		4. The boss room overrides any tiles it is placed on (normal rooms do not override floor tiles)
		5. The boss room implements several new tiles, which will be used for the path methods later

- Player Mechanics
	-Added movement to the player, along with several different graphics to represent a player facing each direction.
	-Gave the protagonist, Cedar, an avatar! (He is no longer just crabman)
	-Movement uses WASD, as in most games
	-The player is locked in the center of the screen, and the visible area around them changes when they move.
	-The player also has a health bar, visible in the top left corner of the screen.
	-The health bar is filled proportionally based on their health as a percentage of their initial total health. 

- EnTT Progress
	-I figured out that the project does not like C++17 very much, but a C++14 version of EnTT was available.
	-After some research, I figured out how to get the c++14 version from github.
	-I tried some very basic functions of the library, and it appears to be functional.

- Testing
	-I started adding tests for the fundamental map classes, starting out with tile, room, and coordinate.

- Obstacles
	-In placing the player's health bar, I used a lot of magic numbers to get the placement right.
	-Some methods, like hallway generation, required necessary modifiers that made lines of code very long. 
	-I'm having a bit of trouble with the master branch of the project. I'm able to push to another branch but not master.
	-Very often I mistype a function without adding parentheses, which frequently was the cause of error.
	-I had some code incorrect, where I passed values of x,y to y,x functions, but there was little recognizable difference when fixed.
	-The last key part of level generation, ensuring a path from spawn to end, is likely the hardest part. 
		1. I'm mostly set on how I want to detect if a path exists, however I am uncertain of how to actually create one if none exist.
		2. It will likely detect a path by travelling along adjacent passable tiles, trying to reach a special 'gateway' tile
	-ofApp is quite cluttered, mostly with loading the graphics, but I may need to try to condense some functionality.

- Progress
	-A player can walk around a randomly generated map
	-Rooms can be created in and pulled from a text file
	-The player has a health bar that changes based on health


## Week 2

- Map Mechanics
	-I completed the last aspect of the map generation, making sure the spawn point and the boss room are connected
	-The boss room will now represent the end of the level (originally I think some sort of third point was intended)

- Player Mechanics
	-The player can now die, which at this point closes out the game (it will probably be more smooth in the future)
	-The player can enter 'combat mode' which displays the tiles they can attack an enemy from
	-The player can click their mouse to attack an enemy on one of the viable squares (they do not need to be in combat mode) 
	-Now when the player moves or attacks, then enemies on the map take a 'turn' and in turn move or attack themselves.

- Entity Mechanics
	-Using EnTT I created several components to be used for different types of entities.
	-Entities that will exist in the world will have a Location component, representing their current Coordinate (Enemies, Item drops eventually)
	-Enemies have an Enemy component that gives them their default health, damage, and enemy type to determine their graphic
	-Eventually, Items will have several components, like Item, which has general properties and Equipment, which is added to weapons and armor
	-Inventory Items will have an InventoryItem component, and they will mostly be organized outside of the actual Player object.

- Enemy Mechanics
	-Enemies now randomly spawn in the level when it is generated (from a minimum to maximum number)
	-Their type is randomly selected from  the available enemy types (Crabman, Octopus, and Eel)
	-When no longer at full health, a smaller health bar will display over their head
	-Enemies will move randomly each time the player moves
	-When close to the player, enemies will choose from any movement options that close the distance between them and the player, though they have a chance of standing still.
	-When next to the player, the enemy will attack and do damage to the player.


- Obstacles
	-A lot of my code still needs to be cleaned up and organized
	-For example, I need to create more shorthand and utility functions so I'm not frequently repeating code 
	-I also need to create more utility functions to reduce long lines caused by conditionals with lots of conditions
	-I need to test more methods, but I think trying to test ofApp methods might give me some trouble
	-I still need to fix the Github project so that I'm pushing to the correct branch. 
	-I might need to relocated some ofApp functions to the entity file for example, for organization's sake.

- Progress
	-The player can check which squares they can attack from
	-The player can attack enemies, and the player dies if they have no health
	-Enemies spawn randomly in the level, and move randomly when far away
	-Enemies move towards the player when close and attack when adjacent
	-Enemies display a health bar when attacked and disappear when killed