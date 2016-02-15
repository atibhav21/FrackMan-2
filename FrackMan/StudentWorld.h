#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <vector>
#include "GameWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
        currentLevel = getLevel();
        
	}

    /*initialize the game
     *  intialize all the data structures required for the game
     *  construct a new oil field with all goodies and protestors
     *  initialize frackman and put him at correct position
     *  use a vector of type Actor* for all objects except frackman and dirt
     *  generate a separate pointer to frackman
     *  create a 2D array of pointers for Dirt
     *
     */
	virtual int init()
	{
		return GWSTATUS_CONTINUE_GAME;
	}

    //run a single tick of the game
	virtual int move()
	{
		  // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		  // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

    //called when player dies due to his health reaching 0 or a boulder falling on him
    
	virtual void cleanUp()
	{
        
	}

private:
    int currentLevel;
    GraphObject* frackManPointer;
    GraphObject* dirtArray[60][60];
};

#endif // STUDENTWORLD_H_
