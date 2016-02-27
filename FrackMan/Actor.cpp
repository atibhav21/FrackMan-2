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
    if(x> VIEW_WIDTH -4 || y> VIEW_HEIGHT-4 || x< 0 || y< 0)
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
    
    int x = getX();
    int y = getY();
    
    if(getStudentWorld()->checkProtesterDistance(this, x, y, 0 ,2) == true)
    {
        setAlive(false);
        setVisible(false);
        return;
    }
    
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
    if(fallingState == true)
    {
        return true;
    }
    return false;
}

void Boulder::doSomething()
{
    int x = getX();
    int y = getY();
    if(!isAlive())
    {
        return;
    }
    if(checkDirtUnder(x, y) == true && fallingState == false)
    {
        return;
    }
    
    if(getStudentWorld()->checkProtesterDistance(this, x, y, 500,10) == true)
    {
        getStudentWorld()->updateSelectiveGrid(getX(), getY());
        setAlive(false);
        setVisible(false);
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
    
    if(getStudentWorld()->checkProtesterDistance(this, x, y, 500 ,10) == true)
    {
        setAlive(false);
        setVisible(false);
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
        if(checkDirtUnder(x, y) == true || x == 0)
        {
            fallingState = false;
            setVisible(false);
            setAlive(false);
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
Goodie::Goodie(int ImageID, int x, int y, StudentWorld* sw, Direction dir, double size, unsigned int depth, FrackMan* fm, bool inititiallyPickUpAble)
:Actor(ImageID, x, y, sw, dir, size, depth)
{
    setVisible(true);
    setAlive(true);
    FMP = fm;
    isPickUpAble = inititiallyPickUpAble;
}

FrackMan* Goodie::getFrackMan()
{
    return FMP;
}

/*  pickUpAble should be true always for Sonar Kit and Water Pool
 *  returns 0 for pickedUp by frackMan, 1 for protestors, 2 for sunk in
 *        ground, 3 for nugget distance <= 4.0, 4 if object is not alive, -1 if none of the above is valid
 */
int Goodie::activate(bool permanent, int SoundID, int pointsIncrease)
{
    if(isAlive() == false)
    {
        return 4;
    }
    
    double distance = sqrt( pow(FMP->getX() - getX(), 2) + pow(FMP->getY()-getY(), 2) );
    if(permanent == true && isPickUpAble && distance<=3.0) //Change Made
    {
        getStudentWorld()->playSound(SoundID);
        getStudentWorld()->increaseScore(pointsIncrease);
        setAlive(false);
        setVisible(false);
        return 0;
    }
    else if(permanent == true && !isPickUpAble && distance<=4.0) //Change made
    {
        isPickUpAble = true;
        setVisible(true);
        return 3;
    }
    else if(permanent == false && getStudentWorld()->checkProtesterDistance(this, getX(), getY(), pointsIncrease, 0) == true)
    {
        //getStudentWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        setAlive(false);
        setVisible(false);
        return 1;
    }
    return -1;
}





//Sonar Kit Class
SonarKit::SonarKit(int x, int y, StudentWorld* sw, FrackMan* fm)
:Goodie(IID_SONAR, x, y, sw, right, 1.0, 2, fm, true)
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
:Goodie(IID_BARREL, x, y, sw, right, 1.0, 2, fm, false)
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
:Goodie(IID_GOLD, x, y, sw, right, 1.0, 2, fm, false)
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
    int x;
    if(m_temporary == false)
    {
        x = activate(true, SOUND_GOT_GOODIE, 10);
    }
    else
    {
        x = activate(false, SOUND_PROTESTER_FOUND_GOLD, 25);
    }
    
    
    if(x == 4 || x == 3)
    {
        return;
    }
    if(x == 0)
    {
        getFrackMan()->addGold();
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
:Goodie(IID_WATER_POOL, x, y, sw, right, 1.0, 2, fm, true)
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

//Agent Class
Agent::Agent(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, unsigned int hitPoints)
:Actor(imageID, startX, startY,world, startDir, 1.0, 0)
{
    m_hitPoints = hitPoints;
}

bool Agent::canPickThingsUp() const
{
    return true;
}

//returns false if the actor has died. returns true otherwise
bool Agent::annoy(unsigned int amt)
{
    m_hitPoints-= amt;
    if(m_hitPoints <= 0)
    {
        setAlive(false);
        setVisible(false);
        return false;
    }
    return true;
}

bool Agent::canMoveInDirection(Direction d)
{
    int x = getX();
    int y = getY();
    if(d == left && moveToIfPossible(x-1, y))
        return true;
    if(d == right && moveToIfPossible(x+1, y))
        return true;
    if(d == up && moveToIfPossible(x, y+1))
        return true;
    if(d == down && moveToIfPossible(x, y-1))
        return true;
    return false;
}

bool Agent::moveInDirection()
{
    int x = getX();
    int y = getY();
    Direction directionToMoveIn = getDirection();
    if(directionToMoveIn == down)
    {
        if(y == 0)
        {
            moveTo(x, y);
        }
        if(moveToIfPossible(x, y-1) == true)
        {
            
                moveTo(x, y-1);
            
            return true;
        }
    }
    else if(directionToMoveIn == up )
    {
        if( y == 60)
        {
            moveTo(x, y);
        }
        if(moveToIfPossible(x, y+1) == true)
        {
            
                moveTo(x , y+1);
        
            
            return true;
        }
    }
    else if(directionToMoveIn == left)
    {
        if(x == 0)
        {
            moveTo(x, y);
        }
        if(moveToIfPossible(x-1, y)== true)
        {
            
            moveTo(x-1, y);
            return true;
        }
    }
    else if(directionToMoveIn == right)
    {
        if( x == 60)
        {
            moveTo(x, y);
        }
        if(moveToIfPossible(x+1, y)== true)
        {
                moveTo(x+1, y);
            return true;
        }
    }
    return false;
}

//frackMan constructor and methods
FrackMan::FrackMan(int x, int y, StudentWorld* sw)
:Agent(sw, x,y, right, IID_PLAYER, 100)
{
    setDirection(right);
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
        getStudentWorld()->squirtUsed(getDirection());
        
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
    hitPoints -= amt;
    if(hitPoints <= 0)
    {
        setAlive(false);
        setVisible(false);
        getStudentWorld()->playSound(SOUND_PLAYER_GIVE_UP);
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
        if(movePending == true)
        {
            
            moveInDirection();
            movePending = false;
            
        }
        if(getHitPoints() == 0)
        {
            setAlive(false);
            setVisible(false);
            getStudentWorld()->playSound(SOUND_PLAYER_GIVE_UP);
            return;
        }
        
    }
}


int lastPerpendicularTurn;

//Protester Classes
// score is the amt by which frackman's points increase if he kills protester
Protester::Protester(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score)
:Agent(world, startX, startY, left, imageID, hitPoints)
{
    setVisible(true);
    setAlive(true);
    m_hitPoints = hitPoints;
    m_gold = 0;
    level = getStudentWorld()->getLevel();
    ticksToWait = max(0, 3- (level/4));
    ticksSinceLastShout = 15;
    stepsToMove = rand()%53 + 8;
    restingTicks = 0;
    lastPerpendicularTurn = 0;
    leaveOilFieldState = false;
    stunState = false;
    doesDirectionNeedToBeChanged = true;
}

bool Protester::canAnnoyFrackMan() const
{
    if(ticksToWait >0 || leaveOilFieldState || stunState == true || ticksSinceLastShout<15)//|| restingTicks > 0)
    {
        return false;
    }
    return true;
}

bool Protester::annoy(unsigned int amt)
{
    if(amt == 0)
    {
        return false;
    }
    
    if(leaveOilFieldState == true)
    {
        return false;
    }
    
    m_hitPoints-= amt;
    if(m_hitPoints<=0 )
    {
        getStudentWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        getStudentWorld()->increaseScore(100);
        setLeaveOilFieldState();
        return true;
    }
    else
    {
        getStudentWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        setStunState();
    }
    return false;
}

void Protester::changeDirection()
{
    stepsToMove = 0;
    Direction currentDirection = getDirection();
    Direction dir = getDirection();
    if(getStudentWorld()->checkLineOfSight(this) == false)
    {
        do
        {
            int randomNo = rand()%4;
            if(randomNo == 0)
            {
                dir = right;
            }
            else if(randomNo == 1)
            {
                dir = left;
            }
            else if(randomNo == 2)
            {
                dir = up;
            }
            else
            {
                dir = down;
            }
            //set a new direction
            //change steps to move in to random number
        }while(canMoveInDirection(dir) == false);
    }
    setDirection(dir);
    if(isPerpendicular(currentDirection, dir) == true)
    {
        lastPerpendicularTurn = 0;
    }
    stepsToMove = rand()%53 + 8;
}

void Protester::setLeaveOilFieldState()
{
    leaveOilFieldState = true;
}

void Protester::addGold()
{
    m_gold++;
    getStudentWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
}

void Protester::followExitPath()
{
    Direction d = getDirection();
    int prevx, prevy;
    if(d == left)
    {
        prevx = getX()+1;
        prevy = getY();
    }
    else if(d == right)
    {
        prevx = getX()-1;
        prevy = getY();
    }
    else if(d == up)
    {
        prevx = getX();
        prevy = getY()-1;
    }
    else if(d == down)
    {
        prevx = getX();
        prevy = getY()+1;
    }
    getStudentWorld()->getExitDirection(this, getX(), getY(),prevx, prevy, d, doesDirectionNeedToBeChanged);
    setDirection(d);
    //d is passed by reference so value is changed
    /*if(moveInDirection() == false)
    {
        changeDirection();
    }*/
    moveInDirection();
}

void Protester::move()
{
    if(leaveOilFieldState == true)
    {
        restingTicks = 0;
    }
    
    if(m_hitPoints<= 0 && leaveOilFieldState == false)
    {
        setLeaveOilFieldState();
        return;
    }
    
    if(stepsToMove == 0)
    {
        changeDirection();
    }
    
    if(ticksToWait > 0 && restingTicks > 0)
    {
        ticksToWait --; 
        restingTicks--;
        return;
    }
    else if(ticksToWait >0 || restingTicks > 0)
    {
        if(ticksToWait>0)
        {
            ticksToWait -- ;
        }
        else
        {
            restingTicks--;
        }
        return;
    }
    
    if(ticksToWait == 0)
    {
        ticksToWait = max(0, 3- (level/4)) ;
    }
    
    if(restingTicks == 0)
    {
        stunState = false;
    }
    
    if(leaveOilFieldState == true)
    {
        followExitPath();
        if(getX() == 60 && getY() == 60)
        {
            setVisible(false);
            setAlive(false);
        }
        return;
    }

    
    if(leaveOilFieldState == false && getStudentWorld()->getFrackManDistance(getX(), getY())<4.0 && ticksSinceLastShout >= 15 && getStudentWorld()->facingFrackMan(this) == true && stunState == false)/*restingTicks<= 0*/
    {
        lastPerpendicularTurn++;
        getStudentWorld()->playSound(SOUND_PROTESTER_YELL);
        /*getStudentWorld()->annoyFrackMan(getAnnoyancePoints());*/
        ticksSinceLastShout = 0;
        setStunState();
        restingTicks =  max(50, 100- level*10);
        return;
    }
    else
    {
        ticksSinceLastShout++;
        
    }
    
    if(trackFrackMan() == true)
    {
        moveInDirection();
        return;
    }
    
    
    if(getStudentWorld()->checkLineOfSight(this))
    {
        if(canMoveInDirection(getDirection()) == true)
        {
            moveInDirection();
            stepsToMove = 0;
            return;
        }
        
    }
    if(stepsToMove > 0 && canMoveInDirection(getDirection()))
    {
        moveInDirection();
        return;
    }
    else
    {
        changeDirection();
    }
    
    
    Direction d1, d2;
    getPerpendicularDirections(d1, d2);
    Direction newDirection = getDirection();
    if(lastPerpendicularTurn >= 200)
    {
        if(isViableDirection(d1))
        {
            if(isViableDirection(d2))
            {
                //choose a random direction
                if(rand()%2 == 0)
                {
                    newDirection = d1;
                }
                else
                {
                    newDirection = d2;
                }
            }
            else
            {
                //set direction to d1
                newDirection = d1;
            }
        }
        else if(isViableDirection(d2))
        {
            if(!isViableDirection(d1))
            {
                //setDirection to d2
                newDirection = d2;
            }
        }
        
        setDirection(newDirection);
        stepsToMove = rand()%53 + 8 ;
        lastPerpendicularTurn = 0;
    }
    else
    {
        lastPerpendicularTurn++;
    }
    
    moveInDirection();
}

bool Protester::isViableDirection(Direction d) 
{
    int x = getX();
    int y = getY();
    if( d == left)
    {
        if(moveToIfPossible(x-1, y) == true)
        {
            return true;
        }
    }
    else if(d == right)
    {
        if(moveToIfPossible(x+1, y) == true)
        {
            return true;
        }
    }
    else if(d == up)
    {
        if(moveToIfPossible(x, y+1) == true)
        {
            return true;
        }
    }
    else if(d == down)
    {
        if(moveToIfPossible(x, y-1) == true)
        {
            return true;
        }
    }
    return false;
}

void Protester::setStunState()
{
    restingTicks += max(50, 100 - level * 10);
    stunState = true;
}

void Protester::getPerpendicularDirections(Direction& d1, Direction& d2)
{
    if(getDirection() == up || getDirection() == down)
    {
        d1 = left;
        d2 = right;
    }
    else if(getDirection() == left || getDirection() == right)
    {
        d1 = up;
        d2 = down;
    }
}




bool Protester::isPerpendicular(Direction d1, Direction d2) const
{
    if(d1 == left || d1 == right)
    {
        if(d2 == up || d2 == down)
        {
            return true;
        }
    }
    else if(d1 == up || d1 == down)
    {
        if(d2 == left || d2 == right)
        {
            return true;
        }
    }
    return false;
}

RegularProtester::RegularProtester(StudentWorld* world, int startX, int startY)
:Protester(world, startX, startY, IID_PROTESTER, 5, 20)
{
     //generates a number between 8 and 60 inclusive
    setVisible(true);
    setAlive(true);
}

bool RegularProtester::trackFrackMan()
{
    return false;
}


void RegularProtester::doSomething()
{
    
    if(isAlive() == false)
    {
        return;
    }
    if(getGold() > 0)
    {
        setLeaveOilFieldState();
    }
    move();

    /*
        implementation of squirt: if the object cannot pass through another object, then the first object annoys the second object. In case of boulders, nothing happens but in case of protestors their annoy function is called which would decrease their hit points
     
     */
    
    
}

//StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score

HardCoreProtester::HardCoreProtester(StudentWorld* sw, int x, int y)
:Protester(sw, x, y, IID_HARD_CORE_PROTESTER, 20, 1) 
{
    setVisible(true);
    setAlive(true);
}

bool HardCoreProtester::trackFrackMan()
{
    Direction d = getStudentWorld()->forHardCoreProtester(this);
    if(d == none)
    {
        return false;
    }
    setDirection(d);
    return true;
    //return true; //TODO: Change according to spec
}

void HardCoreProtester::addGold()
{
    setStunState();
    getStudentWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
}

void HardCoreProtester::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    
    move();
    
}
