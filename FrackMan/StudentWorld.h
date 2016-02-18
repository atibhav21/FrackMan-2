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
    StudentWorld(std::string assetDir) ; //constructor

    /*initialize the game
     *  intialize all the data structures required for the game
     *  construct a new oil field with all goodies and protestors
     *  initialize frackman and put him at correct position
     *  use a vector of type Actor* for all objects except frackman and dirt
     *  generate a separate pointer to frackman
     *  create a 2D array of pointers for Dirt
     *
     */
    virtual int init();

    //run a single tick of the game
    virtual int move();

    //called when player dies due to his health reaching 0 or a boulder falling on him
    
    virtual void cleanUp();
    
    //remove dirt from these coordinates
    void removeDirt(int startX, int startY, int endX, int endY);

    virtual ~StudentWorld();
private:
    int currentLevel;
    vector<Actor*> objects;
    vector<Actor*>::iterator objectIterator;
    FrackMan* frackManPointer;
    Dirt* dirtArray[VIEW_WIDTH][VIEW_HEIGHT-4];
};

#endif // STUDENTWORLD_H_
