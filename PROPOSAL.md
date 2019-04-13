# Final Project Proposal

For my final project I would like to make a dungeon-crawler game.
It will have turn-based movement and combat.
For an outside library, I have planned on using [EnTT](https://github.com/skypjack/entt), an entity-component-system library. This aids in the creation of entities for the game.
There are several layers of features I would like to implement, from more simple to more complex goals.

## Basic Functionality

These tasks are essential and central components of the game. 
Most of these should be completed before any other features are implemented.

1. Player Movement and Input
The user being able to interact and use the player character
is one of the most important parts of a game, and one of the first features I'd like to get working.

2. Map Tiles
The smallest part of the level design, the tiles the player will move on to (in a similar manner to Wood the Gathering).
These will be divided up into several categories, the most vital being Passable and NotPassable.
These are tiles players and enemies can/cannot walk on respectively.

3. Rooms and Random Map Generation
I want to be able to randomly create a set of rooms for the level the player will exist in. 
I will likely create several room presets that the generator will choose from. 
The rooms will be placed in random locations, and stitched together with hallways. 
The layout must also be checked, to make sure paths from where the player enters and exits are linked.

4. Player Goal
There will be an exit door that must be unlocked with a key found in the level. This is how the player wins.

Note for most of the visual capabilities I plan on using the Graphics components of openFrameworks.

## Intermediate Functionality

These features are quite important to the game,
but rely on the completion of more basic functionality to exist.

1. Player Mechanics
The player will possess an inventory to hold items. They will also have designated item slots for armor and weapons.
Some items will function simply by being contained within the player inventory, such as magic modifiers.
A levelling up system will improve the player's health and damage.
When the player levels up they can also choose certain rewards, like increasing movement per turn, or a magic spell.
Magic spells will act in a specified behavior per spell. 
The player can only use one spell per turn, and after a certain number, 
they must wait some number of turns for their Magic to recharge.

2. Enemy Behavior
Enemies will be spawned randomly in the level, at a number within a certain range.
There will be several different types of enemies. When the player is not nearby, they will move randomly.
When the player is close by, enemies will approach and then attack the player.

3. Combat Mechanics
The player will engage in turn-based combat with enemies. Similar to traditional turn-based RPGs, digital or tabletop,
the player will move and then engage, followed by enemy movement and engagement. 
If the player dies, the enemies for the level will be re-rolled, but the level will remain the same layout.
When the player kills an enemy, they will gain experience and can level up.

4. Items
Items are dropped from enemies upon death and can be collected by the player.
The majority of items will be weapons the player can choose from that modifies the damage they output.
Items can also include magic damage modifiers or armor that reduces damage done to the player.
They will have randomly selected names from various enums of strings.

## Advanced Functionality

These are features that I would preferably like to complete. 
However, should implementing the previous features turn out to be more difficulty than expected,
these features may not make it into the final game.

1. Bosses
In order to retrieve a key to unlock the exit door and complete the level, a boss must be defeated.
Bosses will have very specific behavior, different from normal enemies. 
Their movement is limited to a single room, and they will not move until the player enters the room.
They will drop the level completion key upon death.

2. Enemy Variants
There will be a random chance that a different version of an enemy will spawn, such as a Fire or Ice version.
These versions may have different attacks, movement, or damage.

3. Multiple levels
When a player gets a key and uses the exit door, they will enter the next level.
At higher levels, more enemies spawn, and they will do more damage and have more health.
Some specified number of levels will exist within the game, most likely 5. 

## Extra Functionality

The following features may range in diffciulty from easy to hard. 
However most of them I do not find essential to my goal for the game, 
and will not be completed unless I complete all of the above functionality with time to spare.

1. Naive-Bayes Magic Casting
The user, when engaging in combat, will be able to draw shapes on the window to activate a spell.
Naive-Bayes will be used to match up the shape with the spell.

2. Boxes
Boxes containing an item will spawn randomly in a level. 

3. Boss variation
Like the Fire, Ice, etc. variations for enemies, but for bosses as well.
If there is extra time, additional bosses might be added.

4. NPCs
Friendly characters that might give the player an item have a chance to be spawned in a level.

