#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    virtual void doSomething() = 0;
    virtual bool isAlive();
    void setAlive(bool value);
    virtual ~Actor() {}
private:
    bool m_isAlive;
};

class FrackMan: public Actor
{
public:
    FrackMan(int x, int y);
    virtual void doSomething();
    int getHitPoints()      { return hitPoints;}
    void setHitPoints(int amt) { hitPoints += amt; }
    int getSquirtsLeft()    { return squirtUnits; }
    void setSquirts(int amt) { squirtUnits += amt; }
    int getSonarCharges()   { return sonarCharges;}
    void setSonarCharges(int amt) {sonarCharges += amt; }
    int getGold()           { return goldNuggets; }
    void setGold(int amt) {goldNuggets += amt; }
    void changePoints(int amt) { points+= amt; }
    void pressKey(int key);
    /*void setMovePending(bool x);
     bool getMovePending();*/
    virtual ~FrackMan() {}
    
private:
    int hitPoints;
    int squirtUnits;
    int sonarCharges;
    int goldNuggets;
    int points;
    bool movePending;
};



class Dirt: public Actor
{
    
public:
    Dirt(int x, int y);
    virtual void doSomething();
    ~Dirt() {}
};

class Goodie: public Actor
{
public:
    Goodie(int imageID, int x, int y, Direction dir, double size, unsigned int depth, FrackMan* fm);
    virtual void doSomething() {}
    virtual int activate(bool pickUpAble);
    FrackMan* getFrackMan();
    virtual ~Goodie() {}
private:
    FrackMan* FMP; //frackman pointer
};

class GoldNugget: public Goodie
{
public:
    GoldNugget(int x, int y, bool visibility, bool pickableByFrackMan, bool permState, FrackMan* fm);
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
    SonarKit(int x, int y, int level, FrackMan* fm);
    virtual void doSomething();
    ~SonarKit() {}
private:
    int tickCount;
};

class WaterPool: public Goodie
{
public:
    WaterPool(int x, int y, int level, FrackMan* fm);
    virtual void doSomething();
    ~WaterPool() {}
private:
    int tickCount;
};


#endif // ACTOR_H_
