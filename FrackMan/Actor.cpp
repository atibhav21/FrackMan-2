#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>

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



//dirt Class
Dirt::Dirt(int x, int y)
            :Actor(IID_DIRT, x, y, right, 0.25, 3)
{
    
}

void Dirt::doSomething()
{
    
    
}

//Goodie Class
Goodie::Goodie(int ImageID, int x, int y, Direction dir, double size, unsigned int depth, FrackMan* fm)
:Actor(ImageID, x, y, dir, size, depth)
{
    FMP = fm;
}

FrackMan* Goodie::getFrackMan()
{
    return FMP;
}

/*  pickUpAble should be true always for Sonar Kit and Water Pool
 *  TODO: Change possibly to int return type with 0 for pickedUp by frackMan, 1 for protestors, 2 for sunk in
 *        ground, 3 for nugget distance <= 4.0, 4 if object is not alive, -1 if none of the above is valid
 */
int Goodie::activate(bool pickUpAble)
{
    if(isAlive() == false)
    {
        return 4;
    }
    double distance = sqrt( (FMP->getX()-getX())^2 + (FMP->getY()-getY())^2 );
    if(isVisible() == false && distance<=4.0)
    {
        setVisible(true);
        return 3;
    }
    else if(pickUpAble == true && distance<=3.0)
    {
        setAlive(false);
        return 0;
    }
    
    return -1;
}



//Gold Nugget Class
GoldNugget::GoldNugget(int x, int y, bool visibility, bool pickableByFrackMan, bool permState, FrackMan* fm)
:Goodie(IID_GOLD, x, y, right, 1.0, 2, fm)
{
    m_pickable = pickableByFrackMan;
    m_permState = permState;
    setVisible(visibility);
    
}

void GoldNugget::doSomething()
{
    if(isAlive() == false)
    {
        return;
    }

}

//Sonar Kit Class
SonarKit::SonarKit(int x, int y, int level, FrackMan* fm)
:Goodie(IID_SONAR, x, y, right, 1.0, 2, fm)
{
    setVisible(true);
    tickCount = max(100, 300 - (10* level) );
}


//TODO: play sound for x == 0
void SonarKit::doSomething()
{
    
    int x = activate(true);
    if(x == 4)
    {
        return;
    }
    if(x == 0)
    {
        getFrackMan()->setSonarCharges(2); //TODO: check if 2 is the correct amount or not
        getFrackMan()->changePoints(75);
    }
    tickCount--;
    if(tickCount <= 0)
    {
        setAlive(false);
        setVisible(false);
        return;
    }
    
}

//Water Pool Class
WaterPool::WaterPool(int x, int y, int level, FrackMan* fm)
:Goodie(IID_WATER_POOL, x, y, right, 1.0, 2, fm)
{
    setVisible(true);
    tickCount = max(100, 300 - (10*level) );
}

void WaterPool::doSomething()
{
    int x = activate(true);
    if(x == 4)
    {
        return;
    }
    if(x == 0)
    {
        getFrackMan()->setSquirts(5);
        getFrackMan()->changePoints(100);
    }
    tickCount--;
    if(tickCount <= 0)
    {
        setAlive(false);
        setVisible(false);
        return;
    }
}

//frackMan constructor and methods
FrackMan::FrackMan(int x, int y)
:Actor(IID_PLAYER, x,y, right, 1.0, 0)
{
    setDirection(right);
    hitPoints = 10;
    goldNuggets = 0;
    sonarCharges = 1;
    squirtUnits = 5;
    movePending = false;
    setAlive(true); //frackman is alive when it is created
}

void FrackMan::pressKey(int key)
{
    movePending = true;
    if(key == KEY_PRESS_LEFT)
    {
        setDirection(left);
    }
    else if(key == KEY_PRESS_RIGHT)
    {
        setDirection(right);
    }
    else if(key == KEY_PRESS_DOWN)
    {
        setDirection(down);
    }
    else if(key == KEY_PRESS_UP)
    {
        setDirection(up);
    }
    else
    {
        movePending = false;
    }
}


/*void FrackMan::setMovePending(bool x)
{
    movePending = x;
}

//check if frackman needs to be moved or not
bool FrackMan::getMovePending()
{
    return movePending;
}*/


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
        if(movePending == true)
        {
            
            if(directionToMoveIn == down)
            {
                if(y!= 0)
                {
                    moveTo(x, y-1);
                   
                }
                else
                {
                    moveTo(x, y);
                   
                }
                movePending = false;
            }
            else if(directionToMoveIn == up )
            {
                if(y!= 60)
                {
                    moveTo(x , y+1);
                    
                }
                else
                {
                    moveTo(x, y);
                    
                }
                movePending = false;
            }
            else if(directionToMoveIn == left)
            {
                if(x!= 0)
                {
                    moveTo(x-1, y);
                }
                else
                {
                    moveTo(x, y);
                }
                movePending = false;
            }
            else if(directionToMoveIn == right)
            {
                if(x!= 60)
                {
                    moveTo(x+1, y);
                }
                else
                {
                    moveTo(x, y);
                }
                movePending = false;
            }
            
        }
        
    }
}



