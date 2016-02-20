#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <vector>
#include "GameWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
using namespace std;

class Actor;
class FrackMan;
class Dirt;
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
    
    //returns distance between (x1, y1) and (x2, y2)
    double distance(int x1, int y1, int x2, int y2);

    //called when player dies due to his health reaching 0 or a boulder falling on him
    
    virtual void cleanUp();
    
    //remove dirt from these coordinates
    void removeDirt(int startX, int startY, int endX, int endY);
    
    //check if there is no dirt between startX, startY 4x4 square
    bool noDirt(int startX, int startY);
    
    //check if there are no objects in 6 units of distance
    bool checkEucDistance(int x, int y);
    
    //add a new Item to the grid
    void addNewItem();
    
    void decBarrels() {barrels--;}
    
    //add barrels to the oil field
    void addBarrels();
    
    //move the frackman
    void moveFrackman();
    
    //use a sonar charge
    void sonarChargeUsed();
    
    //use a squirt
    void squirtUsed(string direction);
    
    //remove all the dead objects after a single tick
    void removeDeadGameObjects();
    
    void findCoordinates(int &x, int &y, bool dirtPresent);

    virtual ~StudentWorld();
private:
    int currentLevel;
    int barrels;
    vector<Actor*> objects;
    vector<Actor*>::iterator objectIterator;
    FrackMan* frackManPointer;
    Dirt* dirtArray[VIEW_WIDTH][VIEW_HEIGHT-4];
};

#endif // STUDENTWORLD_H_
