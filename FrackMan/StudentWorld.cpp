#include "StudentWorld.h"
#include <string>
#include <cstdlib>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
:GameWorld(assetDir)
{
    currentLevel = getLevel();
    
}

/*  initialize the game
 *  intialize all the data structures required for the game
 *  construct a new oil field with all goodies and protestors
 *  initialize frackman and put him at correct position
 *  use a vector of type Actor* for all objects except frackman and dirt
 *  generate a separate pointer to frackman
 *  create a 2D array of pointers for Dirt
 *
 */
int StudentWorld::init()
{
    currentLevel = getLevel();
    frackManPointer = new FrackMan(30,60, this);
    for(int i = 0; i<VIEW_WIDTH; i++) //x direction
    {
        for(int j = 0; j<VIEW_HEIGHT-4; j++) //y direction
        {
            dirtArray[i][j] = new Dirt(i, j, this);
            if(i < 30 || i > 33 ||  j<4) //creates a shaft at x values between 30 and 33
            {
                    dirtArray[i][j]->setVisible(true);
                
            }
            else
            {
                dirtArray[i][j]->setVisible(false); //do not display any dirt at this position
                dirtArray[i][j]->setAlive(false);
            }
            
        }
    }
    
    barrels = min(2+currentLevel, 20);
    nuggets = max(5-currentLevel/2, 2);
    boulders = min(currentLevel/2 + 2, 6);
    addBoulders();
    addBarrels();
    addNuggets();
    return GWSTATUS_CONTINUE_GAME;
}

double StudentWorld::distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow (x1 - x2, 2) + pow(y1 - y2, 2));
}

bool StudentWorld::dirtAt(int x, int y) const
{
    if(dirtArray[x][y]->isAlive() || dirtArray[x][y]->isVisible())
    {
        return true;
    }
    return false;
}

bool StudentWorld::noDirt(int startX, int startY)
{
    if(startX>= VIEW_WIDTH-4 || startY>= VIEW_HEIGHT-8)
    {
        return false;
    }
    for(int i = 0; i<4; i++)
    {
        for(int j = 0; j<4; j++)
        {
            if(dirtArray[startX+i][startY+j]->isVisible() == true)
            {
                return false;
            }
        }
    }
    return true;
}

void StudentWorld::removeDirt(int startX, int startY, int endX, int endY)
{
    if(endX>= 63)
    {
        endX = 63;
    }
    if(endY>= 59)
    {
        endY = 59;
    }
    bool soundToBePlayed = false;
    for(int i = startX; i<= endX; i++)
    {
        for(int j = startY; j<= endY; j++)
        {
            if(dirtArray[i][j]->isVisible())
            {
                dirtArray[i][j]->setVisible(false);
                dirtArray[i][j]->setAlive(false);
                soundToBePlayed = true;
            }
        }
    }
    if(soundToBePlayed == true)
    {
        playSound(SOUND_DIG);
    }
}

void StudentWorld::addNewItem()
{
    //check if new Sonar Kit or WaterPool should be added
    int G = currentLevel*25 + 300;
    if(rand()%G == 100) //1 in G chance
    {
        //add a new item
        if(rand()%5 == 1) //1 in 5 chance
        {
            Actor* newItem = new SonarKit(0, 60, this, frackManPointer);
            objects.push_back(newItem);
        }
        else
        {
            int x, y;
            /*do
            {
                x = rand()%VIEW_WIDTH;
                y = rand()% (VIEW_HEIGHT - 4);
                
            }while(noDirt(x, y) == false);*/
            
            findCoordinates(x, y, false);
            
            Actor* newItem = new WaterPool(x, y, this, frackManPointer);
            objects.push_back(newItem);
        }
    }
}

void formatField(int start, int end, int value, string& result)
{
    for(int i = start; start>= end; i--)
    {
        int digit = value%10;
        result[i] = (char) (digit+'0');
        value = value /10;
        if(value == 0)
        {
            break;
        }
    }
}

string StudentWorld::formatText(int score, int level, int lives, int health, int squirts, int gold, int sonar, int barrelsLeft)
{
    string text;
    
    string scoreText = "Scr: 000000";
    if(score!= 0)
        formatField(10, 5, score, scoreText);
    
    string levelText = "Lvl: 0";
    if(level!= 0)
        formatField(5, 4, level, levelText);
    
    string livesText = "Lives: 0";
    livesText[7] = (char)(lives+'0');
    
    //TODO: Health field
    string healthText = "Hlth: 000%";
    
    string waterText = "Wtr: 0";
    if(squirts!= 0)
        formatField(5, 4, squirts, waterText);
    
    string goldText = "Gld: 0";
    if(gold!= 0)
        formatField(5, 4, gold, goldText);
    
    string sonarText = "Sonar: 0";
    if(sonar!= 0)
        formatField(7, 6, sonar, sonarText);
    
    string oilText = "Oil Left: 0";
    if(barrelsLeft!=0)
        formatField(10, 9, barrelsLeft, oilText);
    
    text = scoreText + "  " + levelText + "  " + livesText + "  " + healthText + "  " + waterText + "  " + goldText + "  " + sonarText + "  " + oilText;
    
    return text;
}

void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int health = frackManPointer->getHealth();
    int squirts = frackManPointer->getSquirtsLeft();
    int gold = frackManPointer->getGold();
    int sonar = frackManPointer->getSonarCharges();
    int barrelsLeft = barrels;
    
    string s = formatText(score, level, lives, health, squirts, gold, sonar, barrelsLeft);
    setGameStatText(s);
}

void StudentWorld::removeDeadGameObjects()
{
    for(int i = 0; i<objects.size(); i++)
    {
        if(objects[i]->isAlive() == false)
        {
            delete objects[i];
            objects.erase(objects.begin()+ i);
            i--; //do not increment i since next object is moved up
        }
    }
    /*for(int i = 0; i< VIEW_WIDTH; i++)
    {
        for(int j = 0; j<VIEW_HEIGHT-4; j++)
        {
            if(dirtArray[i][j]!= nullptr && dirtArray[i][j]->isAlive() == false)
            {
                delete dirtArray[i][j];
                dirtArray[i][j] = nullptr;
            }
        }
    }*/
}

bool StudentWorld::checkEucDistance(int x, int y)
{
    for(vector<Actor*>::iterator i = objects.begin(); i!= objects.end(); i++)
    {
        if(distance((*i)->getX(), (*i)->getY(), frackManPointer->getX(), frackManPointer->getY()) <= 6.0)
        {
            return false;
        }
    }
    return true;
}

//find empty coordinates according to the spec
void StudentWorld::findCoordinates(int &x, int &y, bool dirtPresent)
{
    do
    {
        x = rand()%(VIEW_WIDTH-4);
        y = rand()% (VIEW_HEIGHT - 4);
        
    }while(noDirt(x, y) == dirtPresent || checkEucDistance(x, y) == false);
}

//add a barrel to the game
void StudentWorld::addBarrels()
{
    for(int i = 0; i<barrels; i++)
    {
        int x, y;
        do
        {
            findCoordinates(x, y, true);
        }while(noDirt(x, y) == true);
        Actor* newItem = new Barrel(x, y, this, frackManPointer);
        objects.push_back(newItem);
    }
}

//add gold nuggets to the game
void StudentWorld::addNuggets()
{
    for(int i = 0; i<nuggets; i++)
    {
        int x, y;
        do
        {
            findCoordinates(x, y, true);
        }while(noDirt(x, y) == true);
        Actor* newItem = new GoldNugget(x, y, this, false, frackManPointer);
        objects.push_back(newItem);
    }
}

//add boulders to the game
void StudentWorld::addBoulders()
{
    for(int i = 0; i<boulders; i++)
    {
        int x, y;
        do
        {
            findCoordinates(x, y, true);
        }while(noDirt(x, y) == true);
        Actor* newItem = new Boulder(x, y, this);
        removeDirt(x, y, x+3, y+3);
        objects.push_back(newItem);
    }
}

void StudentWorld::moveFrackman()
{
    //do something with FrackMan
    int keyPressed;
    getKey(keyPressed);
    frackManPointer->pressKey(keyPressed);
    frackManPointer->doSomething();
    int frackX = frackManPointer->getX();
    int frackY = frackManPointer->getY();
    removeDirt(frackX, frackY, frackX+3, frackY+3);
}

void StudentWorld::sonarChargeUsed()
{
    playSound(SOUND_SONAR);
    for(vector<Actor*>::iterator i = objects.begin(); i!= objects.end(); i++)
    {
        if(distance((*i)->getX(), (*i)->getY(), frackManPointer->getX(), frackManPointer->getY()) <= 12.0)
        {
            (*i)->setVisible(true);
        }
    }
}

void StudentWorld::squirtUsed(string direction)
{
    //TODO: CHECK FOR CORRECT INITIALIZATION POSITION
    playSound(SOUND_PLAYER_SQUIRT);
    Actor* newSquirt;
    if(direction == "left")
    {
        newSquirt = new Squirt(frackManPointer->getX(), frackManPointer->getY(),this, frackManPointer->getDirection());
        
    }
    else if(direction == "right")
    {
        newSquirt = new Squirt(frackManPointer->getX(), frackManPointer->getY(), this, frackManPointer->getDirection());
    }
    else if(direction == "up")
    {
        newSquirt = new Squirt(frackManPointer->getX(), frackManPointer->getY(), this, frackManPointer->getDirection());
    }
    else
    {
        newSquirt = new Squirt(frackManPointer->getX(), frackManPointer->getY(), this, frackManPointer->getDirection());
    }
    objects.push_back(newSquirt);
}

void StudentWorld::dropNugget()
{
    Actor* newItem = new GoldNugget(frackManPointer->getX(), frackManPointer->getY(), this, true, frackManPointer);
    objects.push_back(newItem);
}

bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const
{
    //TODO: Change the implementation of dirt
    //TODO: Falling down of boulder
    if(dirtAt(x, y) == true && a->canDigThroughDirt() == false)
    {
        return false;
    }
    for(int i = 0; i<objects.size(); i++)
    {
        if(objects[i] == a)
        {
            continue;
        }
        if(fabs(x - objects[i]->getX()) < 4.0 && fabs(y - objects[i]->getY()) < 4.0)
        {
            if(objects[i]->canActorsPassThroughMe() == false)
            {
                return false;
            }
        }
    }
    return true;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    setDisplayText();
    
    moveFrackman();
    for(int i = 0; i<objects.size(); i++)
    {
        if(objects[i]->isAlive())
        {
            objects[i]->doSomething();
            
            if(frackManPointer->isAlive() == false)
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if(barrels == 0)
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }

        }
    }
    addNewItem();
    
    removeDeadGameObjects();
    
    if(frackManPointer->isAlive() == false)
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if(barrels == 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
    /*decLives();
    return GWSTATUS_PLAYER_DIED;*/
}


void StudentWorld::cleanUp()
{
    for(int i = 0; i< VIEW_WIDTH; i++)
    {
        for(int j = 0; j<VIEW_HEIGHT-4; j++)
        {
            if(dirtArray[i][j]!= nullptr)
            {
                dirtArray[i][j]->setAlive(false);
                dirtArray[i][j]->setVisible(false);
                delete dirtArray[i][j];
            }
            
        }
    }
    /*while(objectIterator!= objects.end())
    {
        vector<Actor*>::iterator temp = objectIterator;
        objectIterator++;
        delete temp;
    }*/
    for(int i = 0; i<objects.size(); i++)
    {
        delete objects[i];
        objects.erase(objects.begin()+i);
        i--;
    }
    delete frackManPointer;
}

StudentWorld::~StudentWorld()
{
    for(int i = 0; i<VIEW_WIDTH; i++)
    {
        for(int j = 0; j<VIEW_HEIGHT-4; j++)
        {
            if(dirtArray[i][j] != nullptr)
            {
                delete dirtArray[i][j];
            }
            
        }
    }
    for(int i = 0; i<objects.size(); i++)
    {
        if(objects[i]->isAlive() == false)
        {
            delete objects[i];
            objects.erase(objects.begin()+i);
            i--;
        }
    }
    delete frackManPointer;
}