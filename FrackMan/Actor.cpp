#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor Constructor
Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
:GraphObject(imageID,startX, startY,dir,size,depth)
{
    
}

//dirt constructor
Dirt::Dirt(int x, int y)
            :Actor(IID_DIRT, x, y, right, 0.25, 3)
{
    setVisible(true);
}

void Dirt::doSomething()
{
    
}

//frackMan constructor and methods
FrackMan::FrackMan()
:Actor(IID_PLAYER, 29, 59, right, 1.0, 0)
{
    m_direction = right;
    hitPoints = 10;
    goldNuggets = 0;
    sonarCharges = 1;
    squirtUnits = 5;
    setVisible(true);
}

void FrackMan::doSomething()
{
    
}



