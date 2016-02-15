#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor Constructor
Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
:GraphObject(imageID,startX, startY,dir,size,depth)
{
    setVisible(true);
    m_isAlive = false;
}

bool Actor::isAlive()
{
    return m_isAlive;
}

void Actor::setAlive(bool value)
{
    m_isAlive = value;
}

//dirt constructor
Dirt::Dirt(int x, int y)
            :Actor(IID_DIRT, x, y, right, 0.25, 3)
{
    
}

void Dirt::doSomething()
{
    
}

//frackMan constructor and methods
FrackMan::FrackMan(int x, int y)
:Actor(IID_PLAYER, x,y, right, 1.0, 0)
{
    m_direction = right;
    hitPoints = 10;
    goldNuggets = 0;
    sonarCharges = 1;
    squirtUnits = 5;
    setAlive(true); //frackman is alive when it is created
}

void FrackMan::doSomething()
{
    if(!isAlive()) //frackman is not alive
    {
        return;
    }
    else
    {
        int x = getX();
        int y = getY();
        Direction directionToMoveIn = getDirection(); //TODO: call setDirection where user input is being received
        if(directionToMoveIn == down && y!= 0)
        {
            moveTo(x-1, y);
        }
        else if(directionToMoveIn == up && y!= 59)
        {
            moveTo(x+1 , y);
        }
        else if(directionToMoveIn == left && x!= 0)
        {
            moveTo(x, y-1);
        }
        else if(directionToMoveIn == right && x!= 59)
        {
            moveTo(x, y+1);
        }
    }
}



