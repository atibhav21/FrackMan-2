#include "StudentWorld.h"
#include <string>
#include <cstdlib>
#include <queue>
#include <algorithm>
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
    
    for(int i = 0; i<64; i++)
    {
        for(int j = 0; j<64; j++)
        {
            exitGrid[i][j].count = 1000; //since count of any point on grid cannot be greater than 1000;
        }
    }
    
    ticksSinceLastProtester = 0;
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
    ticksSinceLastProtester = 0;
    protestersOnField = 0;
    frackManPointer = new FrackMan(30,60, this);
    for(int i = 0; i<VIEW_WIDTH; i++) //x direction
    {
        for(int j = 0; j<VIEW_HEIGHT; j++) //y direction
        {
            dirtArray[i][j] = new Dirt(i, j, this);
            if(j >= 60)
            {
                dirtArray[i][j]->setAlive(false);
                dirtArray[i][j]->setVisible(false);
            }
            else if(i < 30 || i > 33 ||  j<4 ) //creates a shaft at x values between 30 and 33
            {
                    dirtArray[i][j]->setVisible(true);
                    dirtArray[i][j]->setAlive(true);
                
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
    /*Actor* newItem = new HardCoreProtester(this, 60, 60);
    objects.push_back(newItem);*/
    updateExitGrid();
    
    return GWSTATUS_CONTINUE_GAME;
}


double StudentWorld::distance(int x1, int y1, int x2, int y2) const
{
    return sqrt(pow (x1 - x2, 2) + pow(y1 - y2, 2));
}

bool StudentWorld::dirtAt(int x, int y) const
{
    if(x> 60 || y> 60)
        return false;
    if(dirtArray[x][y]->isAlive() ) //|| dirtArray[x][y]->isVisible())
    {
        return true;
    }
    return false;
}

bool StudentWorld::checkProtesterDistance(Actor* a, int x, int y, int increasePoints, int annoyAmt)
{
    for(vector<Actor*>::iterator i = objects.begin(); i!= objects.end(); i++)
    {
        //i cannot be the same as the object calling function
        //i should be able to pick things up
        if((*i)->canPickThingsUp() == true)
        {
            if(distance((*i)->getX(), (*i)->getY(), x, y) <= 4.0)
            {
                //Check if it works properly or not, set in leaveOilFieldState
                (*i)->annoy(annoyAmt);
                if(annoyAmt == 0) // is a Gold Nugget and Protester needs to be set in a bribed state
                {
                    (*i)->addGold();
                }
                increaseScore(increasePoints);
                return true;
            }
        }
        
    }
    return false;
}

//checks if there is any objects at x,y
bool StudentWorld::isEmpty(int x, int y)
{
    if(x>VIEW_WIDTH -4 || y>VIEW_HEIGHT -4 || x< 0 || y< 0)
        return false;
    if(dirtAt(x,y) == true)
    {
        return false;
    }
    for( int i = 0; i<objects.size(); i++)
    {
        if(objects[i]->getX() == x && objects[i]->getY() == y)
        {
            if(! objects[i]->canActorsPassThroughMe())
            {
                return false;
            }
        }
    }
    return true;
}

void StudentWorld::updateSelectiveGrid(int x, int y)
{
    if(exitGrid[x-1 ][y].count!= 1000) //left is empty
    {
        exitGrid[x][y].visited = true;
        if(exitGrid[x][y].count > exitGrid[x-1][y].count + 1)
            exitGrid[x][y].count = exitGrid[x-1][y].count + 1;
    }
    if(exitGrid[x+1 ][y].count!= 1000) //left is empty
    {
        exitGrid[x][y].visited = true;
        if(exitGrid[x][y].count > exitGrid[x+1][y].count + 1)
            exitGrid[x][y].count = exitGrid[x+1][y].count + 1;
    }
    if(exitGrid[x ][y-1].count!= 1000) //left is empty
    {
        exitGrid[x][y].visited = true;
        if(exitGrid[x][y].count > exitGrid[x][y-1].count + 1)
            exitGrid[x][y].count = exitGrid[x][y-1].count + 1;
    }
    if(exitGrid[x][y+1].count!= 1000) //left is empty
    {
        exitGrid[x][y].visited = true;
        if(exitGrid[x][y].count > exitGrid[x][y+1].count + 1)
            exitGrid[x][y].count = exitGrid[x][y+1].count + 1;
    }
}

void StudentWorld::resetExitGrid()
{
    for(int i = 0; i< 64; i++)
    {
        for(int j = 0; j< 64; j++)
        {
            exitGrid[i][j].visited = false;
            exitGrid[i][j].count = 1000;
        }
    }
}

void StudentWorld::updateExitGrid()
{
    //resetExitGrid();
    queue<Cell> m_queue;
    Cell a(0, 60, 60);
    m_queue.push(a);
    while(!m_queue.empty())
    {
        Cell m = m_queue.front();
        m_queue.pop();
        exitGrid[m.x][m.y].count = m.count;
        exitGrid[m.x][m.y].visited = true;
        if(isEmpty(m.x-1, m.y) && exitGrid[m.x-1][m.y].visited == false)
        {
            Cell k(m.count+1, m.x-1, m.y);
            m_queue.push(k);
        }
        if(isEmpty(m.x+1, m.y) && exitGrid[m.x+1][m.y].visited == false)
        {
            Cell k(m.count+1, m.x+1, m.y);
            m_queue.push(k);
        }
        if(isEmpty(m.x, m.y+1) && exitGrid[m.x][m.y+1].visited == false)
        {
            Cell k(m.count+1, m.x, m.y+1);
            m_queue.push(k);
        }
        if(isEmpty(m.x, m.y-1) && exitGrid[m.x][m.y-1].visited == false)
        {
            Cell k(m.count+1, m.x, m.y-1);
            m_queue.push(k);
        }
    }
}

GraphObject::Direction StudentWorld::getPreviousdirection(GraphObject::Direction dir)
{
    if( dir == GraphObject::left)
    {
        return GraphObject::right;
    }
    if(dir == GraphObject::right)
    {
        return GraphObject::left;
    }
    if(dir == GraphObject::up)
    {
        return GraphObject::down;
    }
    if(dir == GraphObject::down)
    {
        return GraphObject::up;
    }
    return GraphObject::right; //code should never reach this point
}

void StudentWorld::getExitDirection(Actor*a, int x, int y, GraphObject::Direction& d)
{
    //GraphObject::Direction prevDir = getPreviousdirection(d); //this is the previous direction of object
    //GraphObject::Direction currentDirection = d;
    if(exitGrid[x-1][y].count < exitGrid[x][y].count && canActorMoveTo(a, x-1, y))
    {
            d = GraphObject::left;
    }
    else if(exitGrid[x+1][y].count < exitGrid[x][y].count && canActorMoveTo(a, x+1, y))
    {
        d = GraphObject::right;
    }
    else if(exitGrid[x][y-1].count< exitGrid[x][y].count && canActorMoveTo(a, x, y-1))
    {
        d = GraphObject::down;

    }
    else if(exitGrid[x][y+1].count < exitGrid[x][y].count && canActorMoveTo(a, x, y+1))
    {
        d = GraphObject::up;
    }
    else
    {
        //d = currentDirection;
        /*while(d == prevDir)
        {
            int x = rand()% 4;
            if( x == 0)
            {
                d = GraphObject::left;
            }
            else if( x == 1)
            {
                d = GraphObject::right;
            }
            else if (x == 2)
            {
                d = GraphObject::up;
            }
            else
            {
                d = GraphObject::down;
            }
        }*/
    }
    
    
    //none of these options are viable so pick a random direction
    //check if x is close to shaft. if it is then find that direction and move in that direction
    /*if(y > 60)
    {
        d = GraphObject::up;
    }
    if(x > 33 && x <36)
    {
        d = GraphObject::left;
    }
    else if(x < 30 && x > 27)
    {
        d = GraphObject::right;
    }*/

    
}

bool StudentWorld::noDirt(int startX, int startY) const
{
    if(startX> VIEW_WIDTH-4 || startY> VIEW_HEIGHT-4)
    {
        return false;
    }
    for(int i = 0; i<4; i++)
    {
        for(int j = 0; j<4; j++)
        {
            if(dirtAt(startX + i, startY + j) == true)
            {
                return false;
            }
        }
    }
    return true;
}

void StudentWorld::annoyFrackMan(int amt)
{
    frackManPointer->annoy(amt);
}

double StudentWorld::getFrackManDistance(int x, int y) const
{
    return distance(x, y, frackManPointer->getX(), frackManPointer->getY());
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
            if(dirtArray[i][j]->isAlive())
            {
                dirtArray[i][j]->setVisible(false);
                dirtArray[i][j]->setAlive(false);
                soundToBePlayed = true;
                updateSelectiveGrid(i, j);
            }
        }
    }
    if(soundToBePlayed == true) //dirt has been removed
    {
        playSound(SOUND_DIG);
        /*cerr<<"#########################"<<endl;
        for(int i = 0; i<64; i++)
        {
            for(int j = 0; j<64; j++)
            {
                cerr<<exitGrid[i][j].count <<" ";
            }
            cerr<<endl;
        }
        cerr<<"####################" <<endl;*/
        
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
            do
            {
                //TODO: FIX
                x = rand()%VIEW_WIDTH;
                y = rand()% (VIEW_HEIGHT - 4);
                
            }while(noDirt(x, y) == false || checkEucDistance(x, y) == false);
            
            Actor* newItem = new WaterPool(x, y, this, frackManPointer);
            objects.push_back(newItem);
        }
    }
}

void StudentWorld::addProtester()
{
    if(ticksSinceLastProtester > 0)
    {
        ticksSinceLastProtester--;
        return;
    }
    int P = min(15, 2 + (int)(currentLevel * 1.5));
    if(protestersOnField < P)
    {
        int probabilityOfHardcore = min(90, currentLevel * 10 + 30);
        if(rand() % probabilityOfHardcore == 20)
        {
            //add a hardcore protester
            Actor* newProtester = new HardCoreProtester(this, 60,60);
            objects.push_back(newProtester);
        }
        else
        {
            //add a regular protester
            Actor* newProtester = new RegularProtester(this, 60, 60);
            objects.push_back(newProtester);
        }
        ticksSinceLastProtester = max(25, 200 - currentLevel);
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
    
    string levelText = "Lvl:  0";
    if(level!= 0)
        formatField(6, 5, level, levelText);
    
    string livesText = "Lives: 0";
    livesText[7] = (char)(lives+'0');
    
    //TODO: Health field
    string healthText = "Hlth:  00%";
    if(health!= 0)
        formatField(8, 6, health*10, healthText);
    
    string waterText = "Wtr:  0";
    if(squirts!= 0)
        formatField(6, 5, squirts, waterText);
    
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
    int health = frackManPointer->getHitPoints();
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
        if(distance((*i)->getX(), (*i)->getY(), x, y) <= 6.0)
        {
            return false;
        }
    }
    return true;
}

/*bool StudentWorld::canBePlaced(int x, int y)
{

    if(checkEucDistance(x, y) == false)
    {
        return false;
    }
    if((x>= 27 && x<= 33 &&y>=4) || x<VIEW_WIDTH-4 || y>VIEW_HEIGHT-8)
    {
        return false;
    }
    return true;
}*/

bool StudentWorld::allDirt(int x, int y) const
{
    for(int i = x; i<=x+3; i++)
    {
        for(int j = y; j<= y+3; j++)
        {
            if(!dirtArray[i][j]->isAlive() )//|| !dirtArray[i][j] ->isVisible())
            {
                return false;
            }
        }
    }
    return true;
}

//find empty coordinates according to the spec
void StudentWorld::findCoordinates(int &x, int &y)
{
 
    do
    {
        x = rand()%(VIEW_WIDTH-4);
        y = rand()% (VIEW_HEIGHT - 8);
        //check if the object can be present in the shaft or top row or not
    }while(allDirt(x, y) == false || checkEucDistance(x, y) == false );
}

//add a barrel to the game
void StudentWorld::addBarrels()
{
    for(int i = 0; i<barrels; i++)
    {
        int x, y;
        findCoordinates(x, y);
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
        findCoordinates(x, y);
        //since gold nuggets to be burried in the dirt
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
        findCoordinates(x, y);
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

void StudentWorld::squirtUsed(GraphObject::Direction d)
{
    //TODO: CHECK FOR CORRECT INITIALIZATION POSITION
    playSound(SOUND_PLAYER_SQUIRT);
    Actor* newSquirt;
    if(d == GraphObject::left)
    {
        newSquirt = new Squirt(frackManPointer->getX() -1 , frackManPointer->getY(),this, d);
        
    }
    else if(d == GraphObject::right)
    {
        newSquirt = new Squirt(frackManPointer->getX()+4, frackManPointer->getY(), this, d);
    }
    else if(d == GraphObject::up)
    {
        newSquirt = new Squirt(frackManPointer->getX(), frackManPointer->getY()+4, this, d);
    }
    else
    {
        newSquirt = new Squirt(frackManPointer->getX(), frackManPointer->getY()-1, this, d);
    }
    objects.push_back(newSquirt);
}

void StudentWorld::dropNugget()
{
    Actor* newItem = new GoldNugget(frackManPointer->getX(), frackManPointer->getY(), this, true, frackManPointer);
    objects.push_back(newItem);
}


bool StudentWorld::canActorMoveTo(Actor* a, int startX, int startY, int endX, int endY) const
{

    //only for protester to track down frackman
    
    if(abs(startX - endX)<4.0) //means you have to move in vertical direction
    {
        if(startY>endY)
        {
            //move down
            for(int i = startY; i>=endY;i-- )
            {
                if(canActorMoveTo(a, startX, i-1)==false)
                {
                    return false;
                }
            }
            return true;
        }
        else if(startY<= endY)
        {
            for(int i = startY; i<= endY;i++ )
            {
                if(canActorMoveTo(a, startX, i+1)==false)
                {
                    return false;
                }
            }
            return true;
        }
        
    }
    if(abs(startY - endY)<4.0)
    {
        if(startX>endX)
        {
            //move left
            for(int i = startX; i>=endX; i--)
            {
                if(canActorMoveTo(a, i-1, startY) == false)
                {
                    return false;
                }
            }
            return true;
        }
        
        else
        {
            for(int i = startX; i<=endX; i++)
            {
                if(canActorMoveTo(a, i+1, startY) == false)
                {
                    return false;
                }
            }
            return true;
        }
        
        
    }
    return false;
}

bool StudentWorld::facingFrackMan(Actor* a) const
{
    if(frackManPointer->getX() >= a->getX() && a->getDirection() == GraphObject::right)
    {
        return true;
    }
    else if (frackManPointer->getX() < a->getX() && a->getDirection() == GraphObject::left)
    {
        return true;
    }
    else if(frackManPointer->getY() >= a->getY() && a->getDirection() == GraphObject::up)
    {
        return true;
    }
    else if(frackManPointer->getY() < a->getY() && a->getDirection() == GraphObject::down)
    {
        return true;
    }
    return false;
}

bool StudentWorld::checkLineOfSight(Actor* a)
{
    int fmx = frackManPointer->getX();
    int fmy = frackManPointer->getY();
    int ax = a->getX();
    int ay = a->getY();
    if(fabs(fmx-ax) <= 4.0 || fabs(fmy-ay) <= 4.0) //TODO: Change to <=4.0
    {
        if(distance(fmx, fmy, ax, ay)>4.0)
        {
            if(canActorMoveTo(a,ax, ay, fmx, fmy))
            {
                if(fabs(fmx-ax)<=4.0 && fmy> ay)
                {
                    a->setDirection(GraphObject::up);
                }
                else if(fabs(fmx-ax)<=4.0 )
                {
                    a->setDirection(GraphObject::down);
                }
                else
                {
                    if(fmx>ax)
                    {
                        a->setDirection(GraphObject::right);
                    }
                    else
                    {
                        a->setDirection(GraphObject::left);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const
{
    if(noDirt(x, y) == false && a->canDigThroughDirt() == false)
    {
        return false;
    }
    for(int i = 0; i<objects.size(); i++)
    {
        if(objects[i] == a)
        {
            continue;
        }
        if(objects[i]->canAnnoyFrackMan() == true && distance(objects[i]->getX(), objects[i]->getY() , frackManPointer->getX(), frackManPointer->getY())<4.0)
        {
            frackManPointer->annoy(objects[i]->getAnnoyancePoints());
        }
        
        if( distance(x,y,objects[i]->getX(), objects[i]->getY())<4.0)
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
    
    
    addProtester();
    
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