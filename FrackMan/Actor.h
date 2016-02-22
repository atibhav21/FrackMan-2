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
    
    StudentWorld* getStudentWorld() const;
    
    // Can other actors pass through this actor?
    virtual bool canActorsPassThroughMe() const;
    
    // Can this actor dig through dirt?
    virtual bool canDigThroughDirt() const;
    
    // Can this actor pick items up?
    virtual bool canPickThingsUp() const;
    
    // Does this actor hunt the FrackMan?
    virtual bool canAnnoyFrackMan() const;
    
    // checks if object can move to x, y
    bool moveToIfPossible(int x, int y);
    
    virtual ~Actor() {}
private:
    bool m_isAlive;
    StudentWorld* studentWorld;
};

class FrackMan: public Actor
{
public:
    FrackMan(int x, int y, StudentWorld* sw);
    virtual void doSomething();
    virtual bool canDigThroughDirt() const {return true;}
    virtual bool canActorsPassThroughMe() const {return false; }
    virtual bool canPickThingsUp() const {return true;}
    
    int getHitPoints()              { return hitPoints;}
    void setHitPoints(int amt)      { hitPoints += amt; }
    int getSquirtsLeft()            { return squirtUnits; }
    void setSquirts(int amt)        { squirtUnits += amt; }
    int getSonarCharges()           { return sonarCharges;}
    void incSonarCharges(int amt)   {sonarCharges += amt; }
    int getGold()                   { return goldNuggets; }
    void setGold(int amt)           {goldNuggets += amt; }
    int getHealth()                 { return health; }
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
    int health;
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

class Protester: public Actor
{
public:
    Protester();
    virtual void doSomething();
    virtual bool canDigThroughDirt() {return true;}
    virtual bool canPickThingsUp() const {return true; }
    virtual ~Protester();
private:
    int health;
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
    Goodie(int imageID, int x, int y, StudentWorld* sw, Direction dir, double size, unsigned int depth, FrackMan* fm );
    virtual void doSomething() {}
    virtual int activate(bool pickUpAble, int SoundID, int pointsIncrease);
    FrackMan* getFrackMan();
    virtual ~Goodie() {}
private:
    FrackMan* FMP; //frackman pointer
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
