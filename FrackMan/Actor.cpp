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

bool Actor::isAlive() const
{
    return m_isAlive;
}

void Actor::setAlive(bool value)
{
    m_isAlive = value;
}

bool Actor::canAnnoyFrackMan() const
{
    return false;
}

bool Actor::canDigThroughDirt() const
{
    return false;
}

StudentWorld* Actor::getStudentWorld() const
{
    return studentWorld;
}

bool Actor::canActorsPassThroughMe() const
{
    return true;
}

bool Actor::canPickThingsUp() const
{
    return false;
}

bool Actor::moveToIfPossible(int x, int y)
{
    if(x>= VIEW_WIDTH || y>= VIEW_HEIGHT - 4 || x< 0 || y< 0)
    {
        return false;
    }
    return getStudentWorld()->canActorMoveTo(this, x, y);
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
    int x = getX();
    int y = getY();
    switch (getDirection()) {
        case left:
            if(moveToIfPossible( x-1, y) == true)
            {
                moveTo(x-1, y);
                travelDistance--;
            }
            else
            {
                setVisible(false);
                setAlive(false);
            }
            break;
        case right:
            if(moveToIfPossible( x+1, y) == true)
            {
                moveTo(x+1, y);
                travelDistance--;
            }
            else
            {
                setVisible(false);
                setAlive(false);
            }
            break;
        case up:
            if(moveToIfPossible( x, y+1) == true)
            {
                moveTo(x, y+1);
                travelDistance--;
            }
            else
            {
                setVisible(false);
                setAlive(false);
            }
            break;
        case down:
            if(moveToIfPossible( x, y-1) == true)
            {
                moveTo(x, y-1);
                travelDistance--;
            }
            else
            {
                setVisible(false);
                setAlive(false);
            }
            break;
        default:
            cerr<<"Error!!"<<endl;
            break;
    }
    
    
}

//Boulder Class
Boulder::Boulder(int x, int y, StudentWorld* sw)
:Actor(IID_BOULDER,x, y, sw, down, 1.0, 1)
{
    setVisible(true);
    setAlive(true);
    stableState = true;
    waitingState = false;
    fallingState = false;
    centerX = getX() + 2.0;
    centerY = getY() + 2.0;
}

void Boulder::getCenter(double& x, double& y)
{
    x = centerX;
    y = centerY;
}

bool Boulder::checkDirtUnder(int x, int y)
{
    for(int i = x; i<= x+3; i++)
    {
        if(getStudentWorld()->dirtAt(i, y-1) == true)
        {
            return true;
        }
    }
    return false;
}

bool Boulder::canActorsPassThroughMe() const
{
    return false;
}

bool Boulder::canAnnoyFrackMan() const
{
    return true;
}

void Boulder::doSomething()
{
    int x = getX();
    int y = getY();
    if(!isAlive())
    {
        return;
    }
    if(checkDirtUnder(x, y) == true)
    {
        return;
    }
    
    // since its reached this point there's no dirt below the boulder
    if(stableState == true && waitingState == false)
    {
        stableState = false;
        waitingState = true;
        tickCount = 30;
        return;
    }
    if(waitingState == true && tickCount>0)
    {
        tickCount--;
        return;
    }
    if(waitingState == true && tickCount <= 0)
    {
        waitingState = false;
        fallingState = true;
        getStudentWorld()->playSound(SOUND_FALLING_ROCK);
    }
    
    if(fallingState == true)
    {
        FrackMan* frackMan = getStudentWorld()->getFrackMan();
        if(checkDirtUnder(x, y) == true || x == 0)
        {
            fallingState = false;
            setVisible(false);
            setAlive(false);
        }
        else if(fabs(x - frackMan->getX() )< 4.0 && frackMan->getY() == y-1)
        {
            setVisible(false);
            setAlive(false);
            frackMan->annoy(100);
        }
        else
        {
            if(moveToIfPossible( getX(), getY()-1) == true)
            {
                moveTo(getX(), getY()-1);
            }
            else
            {
                fallingState = false;
                setVisible(false);
                setAlive(false);
            }
        }
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
    if(pickUpAble == true && isVisible() && distance<=3.0)
    {
        getStudentWorld()->playSound(SoundID);
        getStudentWorld()->increaseScore(pointsIncrease);
        setAlive(false);
        setVisible(false);
        return 0;
    }
    else if(pickUpAble == true && !isVisible() && distance<=4.0)
    {
        setVisible(true);
        return 3;
    }
    return -1;
}





//Sonar Kit Class
SonarKit::SonarKit(int x, int y, StudentWorld* sw, FrackMan* fm)
:Goodie(IID_SONAR, x, y, sw, right, 1.0, 2, fm)
{
    int level = getStudentWorld()->getLevel();
    tickCount = max(100, 300 - (10* level) );
}


void SonarKit::doSomething()
{
    
    int x = activate(true, SOUND_GOT_GOODIE, 75);
    if(x == 0)
    {
        getFrackMan()->incSonarCharges(2);
        return;
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
Barrel::Barrel(int x, int y, StudentWorld* sw, FrackMan* fm)
:Goodie(IID_BARREL, x, y, sw, right, 1.0, 2, fm)
{
    setAlive(true);
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

//Gold Nugget Class
GoldNugget::GoldNugget(int x, int y, StudentWorld* sw, bool temporary, FrackMan* fm)
:Goodie(IID_GOLD, x, y, sw, right, 1.0, 2, fm)
{
    m_temporary = temporary; // if is temporary then pickable by protestor other wise by frackman
    setVisible(temporary);
    setAlive(true);
    if(m_temporary == true)
    {
        tickCount = 100; 
    }
}

void GoldNugget::doSomething()
{
    int x = activate(!m_temporary, SOUND_GOT_GOODIE, 10);
    
    if(x == 4 || x == 3)
    {
        return;
    }
    if(x == 0)
    {
        getFrackMan()->setGold(1);
        return;
    }
    if(m_temporary == true && isAlive() && tickCount> 0)
    {
        tickCount--;
        return;
    }
    if(m_temporary == true && tickCount <= 0)
    {
        setAlive(false);
        setVisible(false);
    }
}


//Water Pool Class
WaterPool::WaterPool(int x, int y, StudentWorld* sw, FrackMan* fm)
:Goodie(IID_WATER_POOL, x, y, sw, right, 1.0, 2, fm)
{
    int level = getStudentWorld()->getLevel();
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
        if(squirtUnits == 0)
        {
            return;
        }
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
    else if(key == KEY_PRESS_TAB)
    {
        if(goldNuggets == 0)
        {
            return;
        }
        getStudentWorld()->dropNugget();
        goldNuggets--;
    }
}

void FrackMan::annoy(int amt)
{
    health -= amt;
    if(health <= 0)
    {
        setAlive(false);
        setVisible(false);
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
                if(moveToIfPossible(x, y-1) == true)
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
                if(moveToIfPossible(x, y+1) == true)
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



