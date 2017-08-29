#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "GameConstants.h"

class StudentWorld;
class Compiler;

/////////////////////////
////   ACTOR    /////////
/////////////////////////

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth);
    
    StudentWorld* getWorld() const;    
    virtual ~Actor();
    void setoldX(int x);
    int getoldX();
    void setoldY(int y);
    int getoldY();
    
    // Action to perform each tick.
    virtual void doSomething() = 0;
    
    // Does this actor block movement?
    virtual bool blocksMovement() const;

    // Is this actor dead?
    virtual bool isDead() const;
    virtual void setDead();//

    virtual bool isBitten();//
    virtual bool isInsect();//
    virtual bool isPoisoned();//
    virtual bool isStunned(); //

    // Is this actor detected by an ant as a pheromone?
    virtual bool isPheromone(int colony) const;
    virtual void increaseStrength();    //for pheromones
    
    virtual bool isMyHill(int colony) const; //
    virtual int getColony() const; //
    virtual bool isAnt();//
    virtual void updateBitten(bool bitten);

    //NOT IMPLEMENTED
    
    // Cause this actor to be be bitten, suffering an amount of damage.
    virtual void getBitten(int amt); //
    
    // Cause this actor to be be poisoned.
    virtual void getPoisoned();
    
    // Cause this actor to be be stunned.
    virtual void getStunned();
    
    // Can this actor be picked up to be eaten?
    virtual bool isEdible() const;
    
    // Is this actor an enemy of an ant of the indicated colony?
    virtual bool isEnemy(int colony);
    
    // Is this actor detected as dangerous by an ant of the indicated colony?
    virtual bool isDangerous() const;
    
    // Is this actor the anthill of the indicated colony?
    virtual bool isAntHill(int colony) const;//
    
    virtual bool isAdult();//

    Actor::Direction randomDir();//
    
    // Adjust this actor's amount of energy upward or downward.
    virtual void updateEnergy(int amt);//

    // Get this actor's amount of energy (for a Pheromone, same as strength).
    virtual int getEnergy() const;//

private:
    StudentWorld *m_world;
    int m_oldX;
    int m_oldY;
};

/////////////////////////
////   PEBBLE    /////////
/////////////////////////

class Pebble : public Actor
{
public:
    Pebble(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
    virtual bool blocksMovement() const;
};

///////////////////////////////
////   ENERGYHOLDER    /////////
//////////////////////////////

class EnergyHolder : public Actor
{
public:
    EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth);
    
    virtual bool isDead() const;
    
    virtual void setDead();
    
    // Get this actor's amount of energy (for a Pheromone, same as strength).
    virtual int getEnergy() const;
    
    // Adjust this actor's amount of energy upward or downward.
    virtual void updateEnergy(int amt);
    
    // Add an amount of food to this actor's location.
    void addFood(int amt);
    
private:
    int m_hp;
};

///////////////////////////////
////   FOOD           /////////
//////////////////////////////

class Food : public EnergyHolder
{
public:
    Food(StudentWorld* sw, int startX, int startY, int energy);
    virtual void doSomething();
    virtual bool isEdible() const;
};

///////////////////////////////
////   ANTHILL     /////////
//////////////////////////////

class AntHill : public EnergyHolder
{
public:
    AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);
    virtual void doSomething();
    virtual bool isMyHill(int colony) const;
    virtual int getColony() const;
    void giveBirth();
    Compiler* getCompiler();

private:
    int m_colony;
    Compiler* ptr_program;

};

///////////////////////////////
////   PHEROMONE     /////////
//////////////////////////////

class Pheromone : public EnergyHolder
{
public:
    Pheromone(StudentWorld* sw, int startX, int startY, int colony);
    virtual void doSomething();
    virtual bool isPheromone(int colony) const;
    virtual void increaseStrength();
private:
    int m_colony;
};

///////////////////////////////
////   TRIGGERABLE     /////////
//////////////////////////////

class TriggerableActor : public Actor
{
public:
    TriggerableActor(StudentWorld* sw, int x, int y, int imageID);
    virtual bool isDangerous() const;
};

///////////////////////////////
////   WATERPOOL     /////////
//////////////////////////////

class WaterPool : public TriggerableActor
{
public:
    WaterPool(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

///////////////////////////////
////   POISON     /////////
//////////////////////////////

class Poison : public TriggerableActor
{
public:
    Poison(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

///////////////////////////////
////   INSECT     /////////
//////////////////////////////

class Insect : public EnergyHolder
{
public:
    Insect(StudentWorld* world, int startX, int startY, int energy, int imageID);
    virtual void doSomething();
    virtual void getPoisoned();
    virtual void getStunned();
    virtual bool isEnemy(int colony);
    void setPoisoned(bool poison);
    virtual bool isPoisoned();
    virtual bool isInsect();
    virtual bool isStunned();
    virtual void setStunned(bool stun);
    void setSleepCount(int count);
    int getSleepCount();
    virtual bool attemptMove(int x, int y);
    virtual void getBitten(int amt);

    
private:
    bool m_poisoned;
    int m_sleepCount;
    bool m_stunned;
};

///////////////////////////////
////   ANT     /////////
//////////////////////////////


class Ant : public Insect
{
public:
    Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID);
    virtual void doSomething();//
    bool interpreter();//
    bool moveForwardIfPossible(); //
    bool isBitten(); //
    virtual void updateBitten(bool bitten); //
    bool isBlocked();
    void updateBlocked(bool blocked);
    void addPheromone(); //
    Actor::Direction rotateCcw(); //
    Actor::Direction rotateCw(); //
    bool isItTrue(int operand); //
    bool isPheromoneAt(int x, int y, int colony); //
    virtual int getColony() const; //
    virtual bool isAnt(); //
    virtual bool isEnemy(int colony); //
    
private:
    int m_colony;
    Compiler* ptr_program;
    int m_food;
    int m_random;
    int ic;
    bool m_blocked;
    bool m_bitten;
};

///////////////////////////////
////   GRASSHOPPER     /////////
//////////////////////////////

class Grasshopper : public Insect
{
public:
    Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID);
    void setDesiredDistance(int distance);
    int getDesiredDistance();
private:
    int m_desired;
};

////////////////////////////////////
////  BABY GRASSHOPPER     /////////
///////////////////////////////////

class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();

};

////////////////////////////////////
////  ADULT GRASSHOPPER     /////////
///////////////////////////////////

class AdultGrasshopper : public Grasshopper
{
public:
    AdultGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual void getPoisoned();
    virtual void doSomething();
    virtual void getStunned();
    virtual bool isAdult();


};

#endif // ACTOR_H_
