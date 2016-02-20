#include "StudentWorld.h"
#include <string>
#include <cstdlib>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
:GameWorld(assetDir)
{
    currentLevel = getLevel();
    
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
    currentLevel = getLevel();
    frackManPointer = new FrackMan(30,60, this);
    for(int i = 0; i<VIEW_WIDTH; i++) //x direction
    {
        for(int j = 0; j<VIEW_HEIGHT-4; j++) //y direction
        {
            dirtArray[i][j] = new Dirt(i, j, this);
            if(i < 30 || i > 33 ||  j<4) //creates a shaft at x values between 30 and 33
            {
                    dirtArray[i][j]->setVisible(true);
                
            }
            else
            {
                dirtArray[i][j]->setVisible(false); //do not display any dirt at this position
            }
            
        }
    }
    barrels = min(2+currentLevel, 20);
    addBarrels();
    return GWSTATUS_CONTINUE_GAME;
}

double StudentWorld::distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow (x1 - x2, 2) + pow(y1 - y2, 2));
}

bool StudentWorld::noDirt(int startX, int startY)
{
    if(startX>= VIEW_WIDTH-4 || startY>= VIEW_HEIGHT-8)
    {
        return false;
    }
    for(int i = 0; i<4; i++)
    {
        for(int j = 0; j<4; j++)
        {
            if(dirtArray[startX+i][startY+j]->isVisible() == true)
            {
                return false;
            }
        }
    }
    return true;
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
                dirtArray[i][j]->setAlive(false);
                playSound(SOUND_DIG);
            }
        }
    }
}

void StudentWorld::addNewItem()
{
    //check if new Sonar Kit or WaterPool should be added
    int G = currentLevel*25 + 300;
    if(rand()%G == 100) //1 in G chance
    {
        //add a new item
        if(rand()%5 == 1) //1 in 5 chance
        {
            Actor* newItem = new SonarKit(0, 60, this, currentLevel, frackManPointer);
            objects.push_back(newItem);
        }
        else
        {
            int x, y;
            /*do
            {
                x = rand()%VIEW_WIDTH;
                y = rand()% (VIEW_HEIGHT - 4);
                
            }while(noDirt(x, y) == false);*/
            
            findCoordinates(x, y, false);
            
            Actor* newItem = new WaterPool(x, y, this, currentLevel, frackManPointer);
            objects.push_back(newItem);
        }
    }
}

void StudentWorld::removeDeadGameObjects()
{
    for(int i = 0; i<objects.size(); i++)
    {
        if(objects[i]->isAlive() == false)
        {
            delete objects[i];
            objects.erase(objects.begin()+ i);
            i--; //do not increment i since next object is moved up
        }
    }
}

bool StudentWorld::checkEucDistance(int x, int y)
{
    for(vector<Actor*>::iterator i = objects.begin(); i!= objects.end(); i++)
    {
        if(distance((*i)->getX(), (*i)->getY(), frackManPointer->getX(), frackManPointer->getY()) <= 6.0)
        {
            return false;
        }
    }
    return true;
}

//find empty coordinates according to the spec
void StudentWorld::findCoordinates(int &x, int &y, bool dirtPresent)
{
    do
    {
        x = rand()%(VIEW_WIDTH-4);
        y = rand()% (VIEW_HEIGHT - 4);
        
    }while(noDirt(x, y) == dirtPresent || checkEucDistance(x, y) == false);
}

//add a barrel to the game
void StudentWorld::addBarrels()
{
    for(int i = 0; i<barrels; i++)
    {
        int x, y;
        findCoordinates(x, y, true);
        Actor* newItem = new Barrel(x, y, this, currentLevel, frackManPointer); 
        objects.push_back(newItem);
    }
}

void StudentWorld::moveFrackman()
{
    //do something with FrackMan
    int keyPressed;
    getKey(keyPressed);
    frackManPointer->pressKey(keyPressed);
    frackManPointer->doSomething();
    int frackX = frackManPointer->getX();
    int frackY = frackManPointer->getY();
    removeDirt(frackX, frackY, frackX+3, frackY+3);
}

void StudentWorld::sonarChargeUsed()
{
    playSound(SOUND_SONAR);
    for(vector<Actor*>::iterator i = objects.begin(); i!= objects.end(); i++)
    {
        if(distance((*i)->getX(), (*i)->getY(), frackManPointer->getX(), frackManPointer->getY()) <= 12.0)
        {
            (*i)->setVisible(true);
        }
    }
}

void StudentWorld::squirtUsed(string direction)
{
    //TODO: CHECK FOR CORRECT INITIALIZATION POSITION
    playSound(SOUND_PLAYER_SQUIRT);
    Actor* newSquirt;
    if(direction == "left")
    {
        newSquirt = new Squirt(frackManPointer->getX(), frackManPointer->getY(),this, frackManPointer->getDirection());
        
    }
    else if(direction == "right")
    {
        newSquirt = new Squirt(frackManPointer->getX()+4, frackManPointer->getY(), this, frackManPointer->getDirection());
    }
    else if(direction == "up")
    {
        newSquirt = new Squirt(frackManPointer->getX(), frackManPointer->getY()+4, this, frackManPointer->getDirection());
    }
    else
    {
        newSquirt = new Squirt(frackManPointer->getX(), frackManPointer->getY(), this, frackManPointer->getDirection());
    }
    objects.push_back(newSquirt);
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    moveFrackman();
    for(int i = 0; i<objects.size(); i++)
    {
        if(objects[i]->isAlive())
        {
            objects[i]->doSomething();
            
            if(frackManPointer->isAlive() == false)
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if(barrels == 0)
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }

        }
    }
    addNewItem();
    
    removeDeadGameObjects();
    
    if(frackManPointer->isAlive() == false)
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if(barrels == 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
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
    /*while(objectIterator!= objects.end())
    {
        vector<Actor*>::iterator temp = objectIterator;
        objectIterator++;
        delete temp;
    }*/
    for(int i = 0; i<objects.size(); i++)
    {
        delete objects[i];
        objects.erase(objects.begin()+i);
        i--;
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
    for(int i = 0; i<objects.size(); i++)
    {
        if(objects[i]->isAlive() == false)
        {
            delete objects[i];
            objects.erase(objects.begin()+i);
            i--;
        }
    }
    delete frackManPointer;
}