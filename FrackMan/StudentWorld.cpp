#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
:GameWorld(assetDir)
{
    currentLevel = 0;
}

/*  initialize the game
 *  intialize all the data structures required for the game
 *  construct a new oil field with all goodies and protestors
 *  initialize frackman and put him at correct position
 *  use a vector of type Actor* for all objects except frackman and dirt
 *  generate a separate pointer to frackman
 *  create a 2D array of pointers for Dirt
 *
 */
int StudentWorld::init()
{
    frackManPointer = new FrackMan(29, 59);
    for(int i = 0; i<60; i++) //x direction
    {
        for(int j = 0; j<60; j++) //y direction
        {
            dirtArray[i][j] = new Dirt(i, j);
            if(i != 29 && i!= 30)
            {
                dirtArray[i][j]->setVisible(true);
            }
            else
            {
                dirtArray[i][j]->setVisible(false); //do not display any dirt at this position
            }
            
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::removeDirt(int startX, int startY, int endX, int endY)
{
    
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    decLives();
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
    for(int i = 0; i<60; i++)
    {
        for(int j = 0; j<60; j++)
        {
            delete dirtArray[i][j];
        }
    }
    delete frackManPointer;
}

StudentWorld::~StudentWorld()
{
    for(int i = 0; i<60; i++)
    {
        for(int j = 0; j<60; j++)
        {
            delete dirtArray[i][j];
        }
    }
    delete frackManPointer;
}