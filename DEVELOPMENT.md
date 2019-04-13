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