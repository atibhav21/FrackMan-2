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
    frackManPointer = new FrackMan(30, 60);
    for(int i = 0; i<VIEW_WIDTH; i++) //x direction
    {
        for(int j = 0; j<VIEW_HEIGHT-4; j++) //y direction
        {
            dirtArray[i][j] = new Dirt(i, j);
            if(i < 30 || i > 33) //creates a shaft at x values between 30 and 33
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
    if(endX>= 63)
    {
        endX = 63;
    }
    if(endY>= 59)
    {
        endY = 59;
    }
    for(int i = startX; i<= endX; i++)
    {
        for(int j = startY; j<= endY; j++)
        {
            if(dirtArray[i][j]->isVisible())
            {
                dirtArray[i][j]->setVisible(false);
                playSound(SOUND_DIG);
            }
        }
    }
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    //do something with FrackMan
    int keyPressed;
    getKey(keyPressed);
    frackManPointer->pressKey(keyPressed);
    frackManPointer->doSomething();
    int frackX = frackManPointer->getX();
    int frackY = frackManPointer->getY();
    removeDirt(frackX, frackY, frackX+3, frackY+3);
    
    return GWSTATUS_CONTINUE_GAME;
    /*decLives();
    return GWSTATUS_PLAYER_DIED;*/
}

void StudentWorld::cleanUp()
{
    for(int i = 0; i< VIEW_WIDTH; i++)
    {
        for(int j = 0; j<VIEW_HEIGHT-4; j++)
        {
            delete dirtArray[i][j];
        }
    }
    delete frackManPointer;
}

StudentWorld::~StudentWorld()
{
    for(int i = 0; i<VIEW_WIDTH; i++)
    {
        for(int j = 0; j<VIEW_HEIGHT-4; j++)
        {
            delete dirtArray[i][j];
        }
    }
    delete frackManPointer;
}