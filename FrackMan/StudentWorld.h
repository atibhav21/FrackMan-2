#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <vector>
#include <queue>
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
    double distance(int x1, int y1, int x2, int y2) const;

    //called when player dies due to his health reaching 0 or a boulder falling on him
    
    virtual void cleanUp();
    
    //returns direction opposite to dir
    GraphObject::Direction getPreviousdirection(GraphObject::Direction dir);
    
    //remove dirt from these coordinates
    void removeDirt(int startX, int startY, int endX, int endY);
    
    //check if there is no dirt between startX, startY 4x4 square
    //returns true if there is dirt in 4x4 grid
    //returns false otherwise
    bool noDirt(int startX, int startY) const;
    
    //check if there are no objects in 6 units of distance
    bool checkEucDistance(int x, int y);
    
    //check if frackman is in protesters line of sight
    bool checkLineOfSight(Actor* a);
    
    //add a new Item to the grid
    void addNewItem();
    
    //checks if a new Protester needs to be added
    void addProtester();
    
    //check the distance of protesters from the gold nugget
    bool checkProtesterDistance(Actor* a, int x, int y, int increasePoints, int annoyAmt);
    
    bool dirtAt(int x,int y) const;
    
    void decBarrels() {barrels--;}
    
    bool facingFrackMan(Actor* a) const;
    
    //set the game status line
    void setDisplayText();
    
    //format the text
    string formatText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrelsLeft);
    
    //can the actor move to location x, y?
    bool canActorMoveTo(Actor* a, int x, int y) const;
    
    bool isEmpty(int x, int y);
    
    bool canActorMoveTo(Actor* a, int startX, int startY, int endX, int endY) const;
    
    void annoyFrackMan(int amt);
    
    double getFrackManDistance(int x, int y) const;
    
    void getExitDirection(Actor* a, int x, int y, GraphObject::Direction& d);
    
    //add barrels to the oil field
    void addBarrels();
    
    //add gold nuggets to the oil field
    void addNuggets();
    
    //add boulders to the field
    void addBoulders();
    
    //move the frackman
    void moveFrackman();
    
    //use a sonar charge
    void sonarChargeUsed();
    
    //use a squirt
    void squirtUsed(GraphObject::Direction d);
    
    //drop a nugget as a bribe
    void dropNugget();
    
    //remove all the dead objects after a single tick
    void removeDeadGameObjects();
    
    //reset the exit grid to its initial state
    void resetExitGrid();
    
    //update the grid only for location x, y
    void updateSelectiveGrid(int x, int y);
    
    //update Exit Grid
    void updateExitGrid();
    
    //returns if 4x4 grid is covered by dirt or not
    bool allDirt(int x, int y) const;
    
    //finds coordinates which are completely covered by dirt
    void findCoordinates(int &x, int &y);

    virtual ~StudentWorld();
private:
    struct Cell
    {
        Cell(int c, int mx, int my): x(mx), y(my), count(c) {}
        int count;
        int x;
        int y;
    };
    
    struct countGrid
    {
        int count;
        bool visited = false;
    };

    int ticksSinceLastProtester;
    int protestersOnField;
    int currentLevel;
    int barrels;
    int nuggets;
    int boulders;
    vector<Actor*> objects;
    FrackMan* frackManPointer;
    countGrid exitGrid[VIEW_WIDTH][VIEW_HEIGHT];
    Dirt* dirtArray[VIEW_WIDTH][VIEW_HEIGHT];
};

#endif // STUDENTWORLD_H_
