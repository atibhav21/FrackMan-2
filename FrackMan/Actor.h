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
    virtual bool isAlive();
    void setAlive(bool value);
    StudentWorld* getStudentWorld();
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
    int getHitPoints()              { return hitPoints;}
    void setHitPoints(int amt)      { hitPoints += amt; }
    int getSquirtsLeft()            { return squirtUnits; }
    void setSquirts(int amt)        { squirtUnits += amt; }
    int getSonarCharges()           { return sonarCharges;}
    void setSonarCharges(int amt)   {sonarCharges += amt; }
    int getGold()                   { return goldNuggets; }
    void setGold(int amt)           {goldNuggets += amt; }
    int getHealth()                 { return health; }
    void decHealth()                { health = health - 20; }
    void pressKey(int key);
    void useSonar();
    /*void setMovePending(bool x);
     bool getMovePending();*/
    virtual ~FrackMan() {}
    
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
    ~Squirt()                   {}
private:
    int travelDistance;
};

class Protester: public Actor
{
public:
    Protester();
    virtual void doSomething();
    virtual ~Protester();
private:
    int health;
};

class Dirt: public Actor
{
    
public:
    Dirt(int x, int y, StudentWorld* sw);
    virtual void doSomething();
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

class GoldNugget: public Goodie
{
public:
    GoldNugget(int x, int y, StudentWorld* sw, bool visibility, bool pickableByFrackMan, bool permState, FrackMan* fm );
    virtual void doSomething();
    ~GoldNugget() {}
private:
    bool m_visibility;
    bool m_pickable;
    bool m_permState;
};

class SonarKit: public Goodie
{
public:
    SonarKit(int x, int y,StudentWorld* sw, int level, FrackMan* fm);
    virtual void doSomething();
    ~SonarKit() {}
private:
    int tickCount;
};

class Barrel: public Goodie
{
public:
    Barrel(int x, int y, StudentWorld* sw, int level, FrackMan* fm);
    virtual void doSomething();
    ~Barrel() {}
};

class WaterPool: public Goodie
{
public:
    WaterPool(int x, int y,StudentWorld* sw, int level, FrackMan* fm);
    virtual void doSomething();
    ~WaterPool() {}
private:
    int tickCount;
};


#endif // ACTOR_H_
