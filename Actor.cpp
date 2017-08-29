     #include "Actor.h"
#include "GameWorld.h"
#include "StudentWorld.h"
#include <cmath>
#include "GameController.h"
#include "Compiler.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/////////////////////////
////   ACTOR    /////////
/////////////////////////

Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth)
:GraphObject(imageID, startX, startY, startDir, depth, 0.35), m_world(world)
{
    setVisible(true);
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

Actor::~Actor()
{
    setVisible(false);
}


void Actor::setoldX(int x)
{
    m_oldX = x;
}

int Actor::getoldX()
{
    return m_oldX;
}

void Actor::setoldY(int y)
{
    m_oldY = y;
}

int Actor::getoldY()
{
    return m_oldY;
}

//functions

bool Actor::blocksMovement() const
{
    return false;       //false for all except pebble
}

bool Actor::isDead() const
{
    return false;       //always false for pebble, questionable for the rest
}

// Can this actor be picked up to be eaten?
bool Actor::isEdible() const
{
    return false;
}

// Is this actor detected by an ant as a pheromone?
bool Actor::isPheromone(int colony) const
{
    return false;
}

void Actor::increaseStrength()
{
}



//getBitten //getBitten //isBitten not used at all

    // Cause this actor to be be bitten, suffering an amount of damage.
void Actor::getBitten(int amt)  //given one 
{
    //NOT IMPLEMENTED

}
    
    // Cause this actor to be be poisoned.
void Actor::getPoisoned()
{
    
}

    // Cause this actor to be be stunned.
void Actor::getStunned()
{
    
}





    // Is this actor an enemy of an ant of the indicated colony?
bool Actor::isEnemy(int colony)
{
    //for now
    return false;
}

    // Is this actor detected as dangerous by an ant of the indicated colony?
bool Actor::isDangerous() const
{
    return false;
    //baska antleri de yazmamiz gerekiyor mu //su an sadece waterpool ve poison 
}

    // Is this actor the anthill of the indicated colony?
bool Actor::isAntHill(int colony) const
{
    //for now
    return false;
}

//extra ish

void Actor::setDead()
{
    
}

// Adjust this actor's amount of energy upward or downward.
void Actor::updateEnergy(int amt)
{}

// Get this actor's amount of energy (for a Pheromone, same as strength).
int Actor::getEnergy() const
{
    return 0;
}

bool Actor::isAnt()
{
    return false; 
}

Actor::Direction Actor::randomDir()
{
    int direction = randInt(1,4);           //random direction
    if (direction == 1)
        return up;
    else if (direction == 2)
        return right;
    else if (direction == 3)
        return down;
    else
        return left;
}

bool Actor::isBitten()
{
    return false;
}

bool Actor::isInsect()
{
    return false;
}

bool Actor::isPoisoned()
{
    return false;
}

bool Actor::isStunned()
{
    return false;
}

bool Actor::isMyHill(int colony) const
{
    return false;
}

int Actor::getColony() const
{
    return -1;
}

bool Actor::isAdult()
{
    return false;
}

void Actor::updateBitten(bool bitten)
{
    
}
/////////////////////////
////   PEBBLE    /////////
/////////////////////////

//  VERY GOOD

Pebble::Pebble(StudentWorld* world, int startX, int startY)
:Actor(world, startX, startY, right, IID_ROCK, 1)
{
}

bool Pebble::blocksMovement() const
{
    return true;
}

void Pebble::doSomething()
{
}

///////////////////////////////
////   ENERGYHOLDER    /////////
//////////////////////////////

EnergyHolder::EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth)
:Actor(sw, startX, startY, startDir, imageID, depth), m_hp(energy)
{
}

void EnergyHolder::setDead()
{
    m_hp = 0;
}

bool EnergyHolder::isDead() const
{
    if (m_hp <= 0)
        //Actor::setVisible(false);
        return true;
    else
        return false;
}

    // Get this actor's amount of energy (for a Pheromone, same as strength).
int EnergyHolder::getEnergy() const
{
    return m_hp;
}

    // Adjust this actor's amount of energy upward or downward.
void EnergyHolder::updateEnergy(int amt)
{
    m_hp += amt;
}


    // Add an amount of food to this actor's location.
void EnergyHolder::addFood(int amt)
{
    Actor* ptr = getWorld()->getEdibleAt(getX(), getY());
    if (ptr != nullptr)
    {
        ptr->updateEnergy(amt);
    }
    else
    {
        ptr = new Food(getWorld(),getX(),getY(),amt);
        getWorld()->addActor(ptr);
    }
    
}

///////////////////////////////
////   FOOD           /////////
//////////////////////////////

// VERY GOOD

Food::Food(StudentWorld* sw, int startX, int startY, int energy)
:EnergyHolder(sw, startX, startY, right, energy, IID_FOOD, 2)
{
}

void Food::doSomething()
{
}

bool Food::isEdible() const
{
    return true;
}

///////////////////////////////
////   ANTHILL     /////////
//////////////////////////////

AntHill::AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program)
:EnergyHolder(sw, startX, startY, right, 8999, IID_ANT_HILL, 2), m_colony(colony), ptr_program(program)
{
}

int AntHill::getColony() const
{
    return m_colony;
}

void AntHill::doSomething()
{
    updateEnergy(-1);
    
    if (getEnergy() == 0)
    {
        setDead();
        return;
    }
    
    if (getWorld()->getEdibleAt(getX(), getY()))
    {
        int parentResource = (getWorld()->getEdibleAt(getX(), getY()))->getEnergy();
        int eating = 0;
        if (parentResource > 10000)
        {
            eating = 10000;
        }
        else
        {
            eating = parentResource;
        }
        
        (getWorld()->getEdibleAt(getX(), getY()))->updateEnergy(-eating);
        updateEnergy(eating);

        if ((getWorld()->getEdibleAt(getX(), getY()))->isDead())
            (getWorld()->getEdibleAt(getX(), getY()))->setDead();
        
        return;
    }
    
    while (getEnergy() >= 2000)    //producing new ant
    {
        giveBirth();
        updateEnergy(-1500);
        getWorld()->setScore(getColony(), 1);
        
    }
    
}


Compiler* AntHill::getCompiler()
{
    return ptr_program;
}

void AntHill::giveBirth()  // used to give birth to a new ant
{
    int tempID = 0;

    switch (getColony()) {
        case 0:
            tempID = IID_ANT_TYPE0;
            break;
        case 1:
            tempID = IID_ANT_TYPE1;
            break;
        case 2:
            tempID = IID_ANT_TYPE2;
            break;
        case 3:
            tempID = IID_ANT_TYPE3;
            break;
        default:
            break;
    }
    Actor* ptr = new Ant(getWorld(), getX(), getY(), getColony(), getCompiler(), tempID);
    getWorld()->addActor(ptr);
}

bool AntHill::isMyHill(int colony) const
{
    if (m_colony == colony)
    {
        return true;
    }
    return false;
}

///////////////////////////////
////   PHEROMONE     /////////
//////////////////////////////

Pheromone::Pheromone(StudentWorld* sw, int startX, int startY, int colony)
:EnergyHolder(sw, startX, startY, right, 256, 11+colony, 2), m_colony(colony)
{
}

void Pheromone::doSomething()   //done
{
    updateEnergy(-1);
    if (getEnergy() == 0)
    {
        setDead();
        return;
    }
}

bool Pheromone::isPheromone(int colony) const
{
    if (m_colony == colony)
    {
        return true;
    }
    return false;
}
    
    // Increase the strength (i.e., energy) of this pheromone.
void Pheromone::increaseStrength()
{
    int currentEnergy = getEnergy();
    int maxEnergy = 768;
    if (currentEnergy <= 512)
        updateEnergy(256);
    else
        updateEnergy(maxEnergy-currentEnergy);
        
}


///////////////////////////////
////   TRIGGERABLE     /////////
//////////////////////////////

TriggerableActor::TriggerableActor(StudentWorld* sw, int x, int y, int imageID)
:Actor(sw, x, y, right, imageID, 2)
{
}


bool TriggerableActor::isDangerous() const
{
    return true;
}

///////////////////////////////
////   WATERPOOL     /////////
//////////////////////////////

// VERY GOOD

WaterPool::WaterPool(StudentWorld* sw, int x, int y):TriggerableActor(sw, x, y, IID_WATER_POOL)
{
}

void WaterPool::doSomething()
{
    getWorld()->stunAllStunnableAt(getX(), getY());
}

///////////////////////////////
////   POISON     /////////
//////////////////////////////

// VERY GOOD

Poison::Poison(StudentWorld* sw, int x, int y):TriggerableActor(sw, x, y, IID_POISON)
{
}

void Poison::doSomething()
{
    getWorld()->poisonAllPoisonableAt(getX(), getY());
}

///////////////////////////////
////   INSECT     /////////
//////////////////////////////

// still changing based on needs of ANT

Insect::Insect(StudentWorld* world, int startX, int startY, int energy, int imageID)
:EnergyHolder(world, startX, startY, randomDir(), energy, imageID, 1), m_poisoned(false), m_sleepCount(0), m_stunned(false)
{
    
}

bool Insect::attemptMove(int x, int y)
{
    return (!getWorld()->canMoveTo(x,y));
}

void Insect::setSleepCount(int count)
{
    m_sleepCount = count;
}


int Insect::getSleepCount()
{
    return m_sleepCount;
}

void Insect::setStunned(bool stun)
{
    m_stunned = stun;
}

bool Insect::isStunned()
{
    return m_stunned;
}

void Insect::getStunned()
{
    if (m_stunned == false)
    {
        setSleepCount(getSleepCount()+2);
    }
    m_stunned = true;
}

void Insect::doSomething()
{
}

void Insect::getBitten(int amt)
{
    updateEnergy(-amt);
}

void Insect::getPoisoned()
{
    if (m_poisoned == false)
    {
        updateEnergy(-150);
    }
    m_poisoned = true;

    if (getEnergy() <= 0)
    {
        setDead();
    }
}

void Insect::setPoisoned(bool poison)
{
    m_poisoned = poison;
}

bool Insect::isPoisoned()
{
    return m_poisoned;
}

bool Insect::isEnemy(int colony) 
{
    return true;
}

bool Insect::isInsect()
{
    return true;
}

///////////////////////////////
////   ANT     /////////
//////////////////////////////

// NOT IMPLEMENTED

Ant::Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID)
:Insect(sw, startX, startY, 1500, imageID), ptr_program(program), m_food(0), m_random(0), ic(0), m_bitten(false), m_blocked(false), m_colony(colony)
{

}

bool Ant::isAnt()
{
    return true;
}

int Ant::getColony() const
{
    return m_colony;
}

void Ant::doSomething()
{
    updateEnergy(-1);

    if (getEnergy() <= 0)
        setDead();
    
    if (isDead())
    {
        addFood(100);
        return;
    }
        
    
    if (getSleepCount() != 0)
    {
        if (getSleepCount() > 0)
        {
            setSleepCount(getSleepCount()-1);
            return;
        }
        if (getSleepCount() == 0)   //When the count reaches zero, the ant is no longer stunned while on this square.
        {
            setStunned(false);
        }
    }
    
    interpreter();

}





bool Ant::interpreter()
{
    Compiler::Command cmd;
    
    for (;;)
    {
        int count = 0;
        if (!(*ptr_program).getCommand(ic, cmd))
        {
            setDead();
            return false;
        }
        
        switch (cmd.opcode) //cmd.opcode
        {
              
                
            case Compiler::moveForward:
                moveForwardIfPossible();
                ic++;
                return true;
                
            case Compiler::eatFood:
                if (m_food >= 100)
                    updateEnergy(100);
                else
                    updateEnergy(m_food);
                ic++;
                return true;
                
            case Compiler::dropFood:
                addFood(m_food);
                m_food = 0;
                ic++;
                return true;
                
            case Compiler::bite:
                if (getWorld()->isEnemyAt(getX(), getY(), getColony(), this))
                {
                    getWorld()->biteEnemy(this, 15);
                }
                ic++;
                return true;
                
            case Compiler::pickupFood:
                if (getWorld()->getEdibleAt(getX(), getY()))
                {
                    int parentResource = (getWorld()->getEdibleAt(getX(), getY()))->getEnergy();
                    int eating = 0;
                    if (parentResource < 400)
                    {
                        eating = parentResource;
                    }
                    else
                    {
                        eating = 400;
                    }
                    
                    if ((m_food+eating) > 1800)
                    {
                        eating = (1800-m_food);
                    }
                   
                    
                    (getWorld()->getEdibleAt(getX(), getY()))->updateEnergy(-eating);
                    updateEnergy(eating);
                    
                    if ((getWorld()->getEdibleAt(getX(), getY()))->isDead())
                        (getWorld()->getEdibleAt(getX(), getY()))->setDead();
                }
                ic++;
                return true;
            
            case Compiler::emitPheromone:
                addPheromone();
                ic++;
                return true;
                
            case Compiler::faceRandomDirection:
                setDirection(randomDir());
                ic++;
                return true;
                
            case Compiler::rotateClockwise:
                setDirection(rotateCw());
                ic++;
                return true;
                
            case Compiler::rotateCounterClockwise:
                setDirection(rotateCcw());
                ic++;
                return true;
                
            case Compiler::generateRandomNumber:
            {
                int upper = 0;
                if ((stoi(cmd.operand1)) == 0)
                {
                    upper = 0;
                }
                else
                {
                    upper = stoi(cmd.operand1)-1;
                }
                m_random = randInt(0, upper);
                ic++;
                count++;
                break;
            }
                
            case Compiler::goto_command:
                ic = stoi(cmd.operand1);
                count++;
                break;
                
            case Compiler::if_command:
            {
                if (isItTrue(stoi(cmd.operand1)))
                {
                    ic = stoi(cmd.operand2);
                }
                else
                {
                    ic++;
                }
                count++;
                break;
            }
            
        }
        if (count >= 10)
        {
            return false;
        }
        break;
    }
    
    return true;
}

bool Ant::isItTrue(int operand)
{
    switch (operand) {
        case 9:
        {
            if (m_random == 0)
                return true;
            break;
        }
        case 3:
        {
            if (m_food != 0)
                return true;
            break;
        }
        case 4:
        {
            if (getEnergy() <= 25)
                return true;
            break;
        }
        case 7:
        {
            if (getWorld()->isEnemyAt(getX(), getY(), getColony(), this))
            {
                return true;
            }
            break;
        }
        case 6:
        {
            if (getWorld()->getEdibleAt(getX(), getY()))
            {
                if ((getWorld()->getEdibleAt(getX(), getY()))->getEnergy() > 0)
                    return true;
            }
            break;
        }
            
        case 5:
        {
            if (getWorld()->myHill(getX(), getY(), m_colony))
            {
                return true;
            }
            break;
        }
            
        case 1:
        {
            Direction forward = getDirection();
            switch (forward) {
                case up:
                    return isPheromoneAt(getX(),getY()+1, m_colony);
                    break;
                case right:
                    return isPheromoneAt(getX()+1,getY(), m_colony);
                    break;
                case down:
                    return isPheromoneAt(getX(),getY()-1, m_colony);
                    break;
                case left:
                    return isPheromoneAt(getX()-1,getY(), m_colony);
                    break;
                default:
                    break;
            }
            break;
        }
           
        case 0:
        {
            Direction forward = getDirection();
            switch (forward) {
                case up:
                    return getWorld()->isDangerAt(getX(), getY()+1, getColony(), this);
                    break;
                case right:
                    return getWorld()->isDangerAt(getX()+1, getY(), getColony(), this);
                    break;
                case down:
                    return getWorld()->isDangerAt(getX(), getY()-1, getColony(), this);
                    break;
                case left:
                    return getWorld()->isDangerAt(getX()-1, getY(), getColony(), this);
                    break;
                default:
                    break;
                }
            break;
        }
            
        case 2:
        {
            if(m_bitten == true)
                return true;
            break;
        }
            
        case 8:
        {
            if (m_blocked == true)
                return true;
            break;
        }
            
        default:
            break;
    }
    return false;
}

bool Ant::isEnemy(int colony)
{
    if (colony == m_colony)
        return false;
    else
        return true;
}

bool Ant::isPheromoneAt(int x, int y, int colony)
{
    Actor* ptr = getWorld()->getPheromoneAt(x, y, colony);
    if (ptr != nullptr)
    {
        return true;
    }
    return false;

}


void Ant::addPheromone()
{
    Actor* ptr = getWorld()->getPheromoneAt(getX(), getY(), m_colony);
    if (ptr != nullptr)
    {
        ptr->increaseStrength();
    }
    else
    {
        ptr = new Pheromone(getWorld(), getX(), getY(), m_colony);
        getWorld()->addActor(ptr);
    }
}



Actor::Direction Ant::rotateCw()
{
    Direction dir = getDirection();
    Direction destination = dir;
    
    switch (dir) {
        case up:
            destination = right;
            break;
        case right:
            destination = down;
            break;
        case down:
            destination = left;
            break;
        case left:
            destination = up;
            break;
        default:
            break;
    }
    return destination;
}

Actor::Direction Ant::rotateCcw()
{
    Direction dir = getDirection();
    Direction destination = dir;
    
    switch (dir) {
        case up:
            destination = left;
            break;
        case right:
            destination = up;
            break;
        case down:
            destination = right;
            break;
        case left:
            destination = down;
            break;
        default:
            break;
    }
    return destination;
}



bool Ant::moveForwardIfPossible()
{
    Direction dir = getDirection();
    
    bool turn;
    
    switch (dir) {
        case up:
            turn = attemptMove(getX(), getY()+1);
            break;
        case right:
            turn = attemptMove(getX()+1, getY());
            break;
        case down:
            turn = attemptMove(getX(), getY()-1);
            break;
        case left:
            turn = attemptMove(getX()-1, getY());
            break;
        default:
            break;
    }
    
    if (!turn)
    {
        updateBlocked(true);
        return false;
    }
    else
    {
        switch (dir) {
            case up:
                moveTo(getX(), getY()+1);
                break;
            case right:
                moveTo(getX()+1, getY());
                break;
            case down:
                moveTo(getX(), getY()-1);
                break;
            case left:
                moveTo(getX()-1, getY());
                break;
            default:
                break;
        }
        updateBlocked(false);
        updateBitten(false);
        setPoisoned(false);
        return true;
    }
    
}

bool Ant::isBitten()
{
    return m_bitten;
}

void Ant::updateBitten(bool bitten)
{
    m_bitten = bitten;
}

bool Ant::isBlocked()
{
    return m_blocked;
}

void Ant::updateBlocked(bool blocked)
{
    m_blocked = blocked;
}

///////////////////////////////
////   GRASSHOPPER     /////////
//////////////////////////////

Grasshopper::Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID)
:Insect(sw, startX, startY, energy, imageID), m_desired(randInt(2, 10))
{
    
}

void Grasshopper::setDesiredDistance(int distance)
{
    m_desired = distance;
}

int Grasshopper::getDesiredDistance()
{
    return m_desired;
}


////////////////////////////////////
////  BABY GRASSHOPPER     /////////
///////////////////////////////////

BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY)
:Grasshopper(sw, startX, startY, 500, IID_BABY_GRASSHOPPER)
{
}

void BabyGrasshopper::doSomething()
{
    updateEnergy(-1);
    
    if (getEnergy() <= 0)
        setDead();
    
    if (isDead())
    {
        addFood(100);
        return;
    }
    
    if (getSleepCount() != 0)
    {
        if (getSleepCount() > 0)
        {
            setSleepCount(getSleepCount()-1);
        }
        return;
    }
    
    if (getEnergy() >= 1600)
    {
        Actor* ptr = new AdultGrasshopper(getWorld(), getX(), getY());
        getWorld()->addActor(ptr);
        setDead();
        addFood(100);
        return;
    }
    
    if (getWorld()->getEdibleAt(getX(), getY()))
    {
        int resource = (getWorld()->getEdibleAt(getX(), getY()))->getEnergy();
        if (resource < 200)
        {
            (getWorld()->getEdibleAt(getX(), getY()))->updateEnergy(-resource);
            updateEnergy(resource);
        }
        else
        {
            (getWorld()->getEdibleAt(getX(), getY()))->updateEnergy(-200);
            updateEnergy(200);
        }

        if ((getWorld()->getEdibleAt(getX(), getY()))->isDead())
            (getWorld()->getEdibleAt(getX(), getY()))->setDead();
                
        if (randInt(0, 1) == 0)
        {
            setSleepCount(2);
            return;
        }
    }

    if (getDesiredDistance() == 0)
    {
        setDirection(randomDir());
        setDesiredDistance(randInt(2, 10));
    }
    
    Direction dir = getDirection();
    
    bool turn;
    
    switch (dir) {
        case up:
            turn = attemptMove(getX(), getY()+1);
            break;
        case right:
            turn = attemptMove(getX()+1, getY());
            break;
        case down:
            turn = attemptMove(getX(), getY()-1);
            break;
        case left:
            turn = attemptMove(getX()-1, getY());
            break;
        default:
            break;
    }
    
    if (!turn)
        setDesiredDistance(0);
    else
    {
        setDesiredDistance(getDesiredDistance()-1);
        switch (dir) {
            case up:
                moveTo(getX(), getY()+1);
                break;
            case right:
                moveTo(getX()+1, getY());
                break;
            case down:
                moveTo(getX(), getY()-1);
                break;
            case left:
                moveTo(getX()-1, getY());
                break;
            default:
                break;
        }
        setPoisoned(false);
        setStunned(false);
    }
    setSleepCount(2);
}

////////////////////////////////////
////  ADULT GRASSHOPPER     /////////
///////////////////////////////////

AdultGrasshopper::AdultGrasshopper(StudentWorld* sw, int startX, int startY)
:Grasshopper(sw, startX, startY, 1600, IID_ADULT_GRASSHOPPER)
{
    setPoisoned(true);
    setStunned(true);
}

bool AdultGrasshopper::isAdult()
{
    return true;
}


void AdultGrasshopper::getPoisoned()
{       //no effect
}

void AdultGrasshopper::getStunned()
{
}

void AdultGrasshopper::doSomething()
{
    updateEnergy(-1);
    
    if (isDead())
    {
        addFood(100);
        return;
    }
    
    if (getSleepCount() != 0)
    {
        if (getSleepCount() > 0)
        {
            setSleepCount(getSleepCount()-1);
        }
        return;
    }
    
    if (randInt(0,2) == 0)
    {
        if(getWorld()->isEnemyAt(getX(), getY(), -1, this))
        {
            getWorld()->biteEnemy(this, 50);
        }
        setSleepCount(2);
        return;
        
    }
    
    if (randInt(0, 9) == 0)
    {
        double PI = 4 * atan(1.0);  //PI IN THE WRONG PLACE

        double radius = randInt(2,10);
        double angle = randInt(0, 360);
        double CoordX = getX() + radius * cos(angle * PI / 180.0);
        double CoordY = getY() + radius * sin(angle * PI / 180.0);

        int newX = CoordX;
        int newY = CoordY;
        
        if (0 < newX && newX < 63 && 0 < newY && newY < 63 && attemptMove(newX, newY))
            moveTo(newX, newY);
        
        setSleepCount(2);
        return;
    }
    
    if (getWorld()->getEdibleAt(getX(), getY()))
    {
        int resource = (getWorld()->getEdibleAt(getX(), getY()))->getEnergy();
        if (resource < 200)
        {
            (getWorld()->getEdibleAt(getX(), getY()))->updateEnergy(-resource);
            updateEnergy(resource);
        }
        else
        {
            (getWorld()->getEdibleAt(getX(), getY()))->updateEnergy(-200);
            updateEnergy(200);
        }
        
        if ((getWorld()->getEdibleAt(getX(), getY()))->isDead())
            (getWorld()->getEdibleAt(getX(), getY()))->setDead();
        
        if (randInt(0, 1) == 0)
        {
            setSleepCount(2);
            return;
        }
    }
    
    if (getDesiredDistance() == 0)
    {
        setDirection(randomDir());
        setDesiredDistance(randInt(2, 10));
    }
    
    Direction dir = getDirection();
    
    bool turn;
    
    switch (dir) {
        case up:
            turn = attemptMove(getX(), getY()+1);
            break;
        case right:
            turn = attemptMove(getX()+1, getY());
            break;
        case down:
            turn = attemptMove(getX(), getY()-1);
            break;
        case left:
            turn = attemptMove(getX()-1, getY());
            break;
        default:
            break;
    }
    
    if (!turn)
        setDesiredDistance(0);
    else
    {
        setDesiredDistance(getDesiredDistance()-1);
        switch (dir) {
            case up:
                moveTo(getX(), getY()+1);
                break;
            case right:
                moveTo(getX()+1, getY());
                break;
            case down:
                moveTo(getX(), getY()-1);
                break;
            case left:
                moveTo(getX()-1, getY());
                break;
            default:
                break;
        }
    }
    
    setSleepCount(2);
}

