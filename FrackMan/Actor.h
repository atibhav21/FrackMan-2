#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    virtual void doSomething() = 0;
    virtual bool isAlive();
    void setAlive(bool value);
private:
    bool m_isAlive;
};

class Dirt: public Actor
{
    
public:
    Dirt(int x, int y);
    virtual void doSomething();
};

class FrackMan: public Actor
{
public:
    FrackMan(int x, int y);
    virtual void doSomething();
    int getHitPoints()      { return hitPoints;}
    int getSquirtsLeft()    { return squirtUnits; }
    int getSonarCharges()   { return sonarCharges;}
    int getGold()           { return goldNuggets; }
    void pressKey(int key);
    /*void setMovePending(bool x);
    bool getMovePending();*/

private:
    int hitPoints;
    int squirtUnits;
    int sonarCharges;
    int goldNuggets;
    bool movePending;
};

#endif // ACTOR_H_
