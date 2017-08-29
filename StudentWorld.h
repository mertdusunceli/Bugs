#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "GameConstants.h"
#include <list>
#include "Actor.h"
#include "Field.h"
#include <sstream>  // defines the type std::ostringstream
#include <iostream>
#include <iomanip>  //right justification
#include "Compiler.h"

class Actor;
//class Compiler;

class StudentWorld : public GameWorld
{
public:
    // my helper functions
    void setDisplayText();
    Field* get_Field();
    // Can an insect move to x,y?
    bool canMoveTo(int x, int y) const;

    //real deal
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    // Add an actor to the world
    void addActor(Actor* a);
    
    bool myHill(int x, int y, int colony);
    // Is the anthill of the indicated colony at x,y?

    // If an item that can be picked up to be eaten is at x,y, return a
    // pointer to it; otherwise, return a null pointer.  (Edible items are
    // only ever going be food.)
    Actor* getEdibleAt(int x, int y) const;
    
    // If a pheromone of the indicated colony is at x,y, return a pointer
    // to it; otherwise, return a null pointer.
    Actor* getPheromoneAt(int x, int y, int colony) const;
    
    // Poison all poisonable actors at x,y.
    bool poisonAllPoisonableAt(int x, int y);
    
    // Stun all stunnable actors at x,y.
    bool stunAllStunnableAt(int x, int y);
    
    // Is an enemy of an ant of the indicated colony at x,y?
    bool isEnemyAt(int x, int y, int colony, Actor* ptr);
    
    // Is something dangerous to an ant of the indicated colony at x,y?
    bool isDangerAt(int x, int y, int colony, Actor* ptr) const;
    
    bool biteEnemy(Actor* me, int biteDamage);

    // Record another ant birth for the indicated colony.
    void setScore(int colony, int amt);
    
    std::string isWinner(int size);
    int isWinnerCount(int size);

    
private:
    int m_tick;
    Field* m_field;
    std::list <Actor*> m_list [64][64];
    int antCount[4];
    std::string antNames[4];
    
};

#endif // STUDENTWORLD_H_



