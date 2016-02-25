#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY,  StudentWorld* sw, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    virtual void doSomething() = 0;
    
    virtual bool isAlive() const;
    
    void setAlive(bool value);
    
    virtual bool annoy(unsigned int amount) {return false;}
    
    StudentWorld* getStudentWorld() const;
    
    // Can other actors pass through this actor?
    virtual bool canActorsPassThroughMe() const;
    
    // Can this actor dig through dirt?
    virtual bool canDigThroughDirt() const;
    
    // Can this actor pick items up?
    virtual bool canPickThingsUp() const;
    
    // Does this actor hunt the FrackMan?
    virtual bool canAnnoyFrackMan() const;
    
    virtual void setLeaveOilFieldState()  {};
    
    virtual int getAnnoyancePoints() const {return 0;}
    
    // checks if object can move to x, y
    bool moveToIfPossible(int x, int y) ;
    
    virtual ~Actor() {}
private:
    bool m_isAlive;
    StudentWorld* studentWorld;
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* world, int startX, int startY, Direction startDir,
          int imageID, unsigned int hitPoints);
    
    // Pick up a gold nugget.
    virtual void addGold() = 0;
    
    // How many hit points does this actor have left?
    unsigned int getHitPoints() const {return m_hitPoints;}
    
    virtual bool annoy(unsigned int amount);
    virtual bool canPickThingsUp() const ;
    virtual bool canMoveInDirection(Direction d);
    virtual void moveInDirection();
private:
    int m_hitPoints;
};

class Protester: public Agent
{
public:
    Protester(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score);
    virtual void doSomething() {}
    
    virtual bool annoy(unsigned int amount);
    virtual void addGold() {m_gold++;}
    virtual bool huntsFrackMan() const {return true; }
    virtual bool canAnnoyFrackMan() const {return true; }
    virtual int getAnnoyancePoints() const {return 0;}
    
    virtual void setStunState() {};
    
    virtual bool isViableDirection(Direction d) ;
    
    // Set state to having given up protest
    virtual void setLeaveOilFieldState() ;
    
    void followExitPath();
    
    //check if protester is in leave oil field state
    bool getMustLeaveOilField() {return leaveOilFieldState; }
    
    // Set number of ticks until next move
    void setTicksToNextMove();
private:
    int m_gold;
    bool leaveOilFieldState;
    int m_hitPoints;
};

class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual void changeDirection();
    virtual int getAnnoyancePoints() const {return 2;}
    virtual bool canAnnoyFrackMan() const;
    virtual void setStunState();
    bool isPerpendicular(Direction d1, Direction d2) const;
    void getPerpendicularDirections(Direction& d1, Direction& d2);
private:
    int stepsToMove;
    
    int ticksToWait;
    int level;
    int ticksSinceLastShout;
    int restingTicks;
    int lastPerpendicularTurn;
};

class FrackMan: public Agent
{
public:
    FrackMan(int x, int y, StudentWorld* sw);
    virtual void doSomething();
    virtual bool canDigThroughDirt() const {return true;}
    virtual bool canActorsPassThroughMe() const {return false; }
    virtual bool canPickThingsUp() const {return true;}
    
    int getHitPoints()              { return hitPoints;}
    int getSquirtsLeft()            { return squirtUnits; }
    void setSquirts(int amt)        { squirtUnits += amt; }
    int getSonarCharges()           { return sonarCharges;}
    void incSonarCharges(int amt)   {sonarCharges += amt; }
    int getGold()                   { return goldNuggets; }
    virtual void addGold()           {goldNuggets += 1; }
    void annoy(int amt);                
    
    void pressKey(int key);
    void useSonar();
    /*void setMovePending(bool x);
     bool getMovePending();*/
    
private:
    int hitPoints;
    int squirtUnits;
    int sonarCharges;
    int goldNuggets;
    int points;
    bool movePending;
};

class Squirt: public Actor
{
public:
    Squirt(int x, int y, StudentWorld* sw, Direction dir);
    virtual void doSomething();
    int getTravelDistance()     {return travelDistance; }

private:
    int travelDistance;
};

class Boulder: public Actor
{
public:
    Boulder(int x, int y, StudentWorld* sw);
    virtual void doSomething();
    virtual bool canActorsPassThroughMe() const ;
    virtual bool canDigThroughDirt() const {return false;}
    virtual bool canAnnoyFrackMan() const;
    virtual int getAnnoyancePoints() const {return 10;}
    void getCenter( double& x, double &y) ;
private:
    bool checkDirtUnder(int x, int y);
    bool stableState;
    bool waitingState;
    bool fallingState;
    int tickCount;
    double centerX;
    double centerY;
    
};

class Dirt: public Actor
{
    
public:
    Dirt(int x, int y, StudentWorld* sw);
    virtual void doSomething();
    virtual bool canActorsPassThroughMe() const {
        return false; }
    ~Dirt() {}
};

class Goodie: public Actor
{
public:
    Goodie(int imageID, int x, int y, StudentWorld* sw, Direction dir, double size, unsigned int depth, FrackMan* fm , bool initiallyPickUpAble);
    virtual void doSomething() {}
    virtual int activate(bool pickUpAble, int SoundID, int pointsIncrease);
    FrackMan* getFrackMan();
    virtual ~Goodie() {}
private:
    FrackMan* FMP; //frackman pointer
    bool isPickUpAble;
};


//pickable only by the frackman
class GoldNugget: public Goodie
{
public:
    GoldNugget( int startX, int startY,StudentWorld* world, bool temporary, FrackMan* fm);
    virtual void doSomething();

private:
    bool m_temporary;
    int tickCount;
};

class SonarKit: public Goodie
{
public:
    SonarKit(int x, int y,StudentWorld* sw, FrackMan* fm);
    virtual void doSomething();

private:
    int tickCount;
};

class Barrel: public Goodie
{
public:
    Barrel(int x, int y, StudentWorld* sw, FrackMan* fm);
    virtual void doSomething();

};

class WaterPool: public Goodie
{
public:
    WaterPool(int x, int y,StudentWorld* sw, FrackMan* fm);
    virtual void doSomething();

private:
    int tickCount;
};


#endif // ACTOR_H_
