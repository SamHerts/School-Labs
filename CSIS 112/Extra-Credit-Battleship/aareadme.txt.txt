Created by: Samuel Herts
LATE CREDIT USED FOR THIS LAB, submitted within 24 hours of original due date
All code came from myself, references were from cplusplus and textbook.


I chose to implement my knowledge by creating objects as the map and giving them overloaded operators and functions to better create the game,
 using 2D array to store the objects, and output them to files, using exception handling to control files, and using parsing
 on user input to make sure you cant break anything



Things implemented:
:Classes - for map tiles, ships, and water
:Inheritance -  ship and water, from tile
:polymorphism - creating multiple objects and calling their pointers
:RNG - gives probability that a tile will have a ship
:Pointers - pointers to objects
:double pointers - array usage
:array class - did not use vectors..even though they are amazing
:operator overload - << operator to output map to file
:File processing - output image of map
: exception handling - ensure file works

INSTRUCTIONS:
A map of randomly placed ships is created as Map.txt, if you cannot find all the ships, open this and cheat.
Enter an x and y coordinate, between 0 and 24 to shoot a torpedo at that location, if it hits, the total number of ships decreases and you get a hit.
 if you miss, try again. spots hit multiple times will indicate as such, though there is no penalty other than time.
Game ends when all ships have been destroyed.