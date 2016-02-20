#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
#include <cmath>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor Constructor
Actor::Actor(int imageID, int startX, int startY, StudentWorld* sw, Direction dir, double size, unsigned int depth)
:GraphObject(imageID,startX, startY,dir,size,depth)
{
    setVisible(true);
    m_isAlive = true;
    studentWorld = sw;
}

bool Actor::isAlive()
{
    return m_isAlive;
}

void Actor::setAlive(bool value)
{
    m_isAlive = value;
}

StudentWorld* Actor::getStudentWorld()
{
    return studentWorld;
}

//dirt Class
Dirt::Dirt(int x, int y, StudentWorld* sw)
            :Actor(IID_DIRT, x, y, sw, right, 0.25, 3)
{
    setAlive(true);
}

void Dirt::doSomething()
{
    
    
}

/*//Protestor Class
Protestor::Protestor()
{
    
}*/

//Squirt Class
Squirt::Squirt(int x, int y, StudentWorld* sw, Direction dir)
:Actor(IID_WATER_SPURT, x, y, sw, dir, 1.0 , 1)
{
    travelDistance = 4;
    setVisible(true);
    setAlive(true);
}

void Squirt::doSomething()
{
    if(isAlive() == false)
    {
        return;
    }
    if(travelDistance == 0)
    {
        setVisible(false);
        setAlive(false);
        return;
    }
    //TODO: Check for protestor, dirt and boulder and edge of grid
    
    switch (getDirection()) {
        case left:
            moveTo(getX()-1, getY());
            travelDistance--;
            break;
        case right:
            moveTo(getX()+1, getY());
            travelDistance--;
            break;
        case up:
            moveTo(getX(), getY()+1);
            travelDistance--;
            break;
        case down:
            moveTo(getX(), getY()-1);
            travelDistance--;
            break;
        default:
            cerr<<"FUCK THIS BUG"<<endl;
            break;
    }
    
    
}

//Goodie Class
Goodie::Goodie(int ImageID, int x, int y, StudentWorld* sw, Direction dir, double size, unsigned int depth, FrackMan* fm)
:Actor(ImageID, x, y, sw, dir, size, depth)
{
    setVisible(true);
    setAlive(true);
    FMP = fm;
}

FrackMan* Goodie::getFrackMan()
{
    return FMP;
}

/*  pickUpAble should be true always for Sonar Kit and Water Pool
 *  returns 0 for pickedUp by frackMan, 1 for protestors, 2 for sunk in
 *        ground, 3 for nugget distance <= 4.0, 4 if object is not alive, -1 if none of the above is valid
 */
int Goodie::activate(bool pickUpAble, int SoundID, int pointsIncrease)
{
    if(isAlive() == false)
    {
        return 4;
    }
    double distance = sqrt( pow(FMP->getX() - getX(), 2) + pow(FMP->getY()-getY(), 2) );
    if(isVisible() == false && distance<=4.0)
    {
        setVisible(true);
        return 3;
    }
    else if(pickUpAble == true && distance<=3.0)
    {
        getStudentWorld()->playSound(SoundID);
        getStudentWorld()->increaseScore(pointsIncrease);
        setAlive(false);
        setVisible(false);
        return 0;
    }
    
    return -1;
}



//Gold Nugget Class
GoldNugget::GoldNugget(int x, int y, StudentWorld* sw, bool visibility, bool pickableByFrackMan, bool permState, FrackMan* fm)
:Goodie(IID_GOLD, x, y, sw, right, 1.0, 2, fm)
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
SonarKit::SonarKit(int x, int y, StudentWorld* sw, int level, FrackMan* fm)
:Goodie(IID_SONAR, x, y, sw, right, 1.0, 2, fm)
{
    tickCount = max(100, 300 - (10* level) );
}


void SonarKit::doSomething()
{
    
    int x = activate(true, SOUND_GOT_GOODIE, 75);
    if(x == 4)
    {
        return;
    }
    if(x == 0)
    {
        getFrackMan()->setSonarCharges(2);
    }
    tickCount--;
    if(tickCount <= 0)
    {
        setAlive(false);
        setVisible(false);
        return;
    }
    
}

//Barrel of Oil Class
Barrel::Barrel(int x, int y, StudentWorld* sw, int level, FrackMan* fm)
:Goodie(IID_BARREL, x, y, sw, right, 1.0, 2, fm)
{
    setVisible(false);
}

void Barrel::doSomething()
{
    int x = activate(true, SOUND_FOUND_OIL, 1000);
    if(x == 4 || x == 3)
    {
        return;
    }
    if(x == 0)
    {
        getStudentWorld()->decBarrels();
    }
    
}



//Water Pool Class
WaterPool::WaterPool(int x, int y, StudentWorld* sw, int level, FrackMan* fm)
:Goodie(IID_WATER_POOL, x, y, sw, right, 1.0, 2, fm)
{
    tickCount = max(100, 300 - (10*level) );
}

void WaterPool::doSomething()
{
    int x = activate(true, SOUND_GOT_GOODIE, 100);
    if(x == 4)
    {
        return;
    }
    if(x == 0)
    {
        getFrackMan()->setSquirts(5);
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
FrackMan::FrackMan(int x, int y, StudentWorld* sw)
:Actor(IID_PLAYER, x,y, sw, right, 1.0, 0)
{
    setDirection(right);
    health = 100;
    hitPoints = 10;
    goldNuggets = 0;
    sonarCharges = 1;
    squirtUnits = 5;
    movePending = false;
    setVisible(true);
    setAlive(true); //frackman is alive when it is created
}

void FrackMan::useSonar()
{
    sonarCharges--;
    getStudentWorld()->sonarChargeUsed();
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
    
    if(key == 'Z' || key == 'z')
    {
        if(getSonarCharges()>0)
        {
            useSonar();
        }
    }
    else if(key == KEY_PRESS_SPACE)
    {
        squirtUnits--;
        if(getDirection() == left)
        {
            getStudentWorld()->squirtUsed("left");
        }
        else if(getDirection() == right)
        {
            getStudentWorld()->squirtUsed("right");
        }
        else if(getDirection() == up)
        {
            getStudentWorld()->squirtUsed("up");
        }
        else
        {
            getStudentWorld()->squirtUsed("down");
        }
        
    }
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
        Direction directionToMoveIn = getDirection();
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
        if(getHealth() == 0)
        {
            setAlive(false);
            return;
        }
        
    }
}



