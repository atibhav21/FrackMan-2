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
    
    // Can this actor dig through dirt?
    virtual bool canDigThroughDirt() const;
    
    // Does this actor hunt the FrackMan?
    virtual bool huntsFrackMan() const;
    
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
    virtual bool canDigThroughDirt() const {return false;}
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
    virtual bool canDigThroughDirt() const {return false;}
    virtual void doSomething();
    ~Dirt() {}
};

class Goodie: public Actor
{
public:
    Goodie(int imageID, int x, int y, StudentWorld* sw, Direction dir, double size, unsigned int depth, FrackMan* fm );
    virtual void doSomething() {}
    virtual bool canDigThroughDirt() const {return false;}
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
    virtual bool canDigThroughDirt() const {return false;}
    ~GoldNugget() {}
private:
    
};


class Bribe: public Goodie
{
public:
    Bribe(int startX, int startY,StudentWorld* world, bool temporary, FrackMan* fm);
    virtual void doSomething();
    virtual bool canDigThroughDirt() const {return false;}
    ~Bribe() {}
private:
    
};

class SonarKit: public Goodie
{
public:
    SonarKit(int x, int y,StudentWorld* sw, int level, FrackMan* fm);
    virtual void doSomething();
    virtual bool canDigThroughDirt() const {return false;}
    ~SonarKit() {}
private:
    int tickCount;
};

class Barrel: public Goodie
{
public:
    Barrel(int x, int y, StudentWorld* sw, int level, FrackMan* fm);
    virtual void doSomething();
    virtual bool canDigThroughDirt() const {return false;}
    ~Barrel() {}
};

class WaterPool: public Goodie
{
public:
    WaterPool(int x, int y,StudentWorld* sw, int level, FrackMan* fm);
    virtual void doSomething();
    virtual bool canDigThroughDirt() const {return false;}
    ~WaterPool() {}
private:
    int tickCount;
};


#endif // ACTOR_H_
