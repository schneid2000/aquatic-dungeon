# Final Project

## Aquatic Dungeon

My final project is a turn-based dungeon-crawler game, in 16-bit style graphics. 

Credit to Press Start for the font used for the in-game text. I also used [Piskel](https://www.piskelapp.com/) for creating the resources.

In the game, you play as the green-hooded protagonist, Cedar! The player wanders through a randomly generated dungeon, 
and they can fight different types enemies and obtain helpful items, like weapons, shields, and health potions! They can even fight a boss to win the game!

Run the game again, and the dungeon will be generated differently, with variations in the number of enemies that spawn. 

Building starts up the game right away. 


## Build Instructions 

This project uses openFrameworks version 0.10.1 and uses Visual Studio 2017. 

It also includes the header-only library [EnTT](https://github.com/skypjack/entt) for the entities and items. 
IMPORTANT: This project uses the C++14 version of EnTT. It will not work with the updated version.

The location of the EnTT library should be included in the Additional Include Directories in the Project Settings.
The Language standard should also be set to C++14.

Debug/Win32 is recommended for the build mode. 