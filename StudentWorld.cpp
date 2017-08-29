 #include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Compiler.h"

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    m_tick = 0;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    
    Compiler *compilerForEntrant0 = nullptr;
    Compiler *compilerForEntrant1 = nullptr;
    Compiler *compilerForEntrant2 = nullptr;
    Compiler *compilerForEntrant3 = nullptr;

    AntHill *ah0 = nullptr;
    AntHill *ah1 = nullptr;
    AntHill *ah2 = nullptr;
    AntHill *ah3 = nullptr;
    
    vector<string> fileNames = getFilenamesOfAntPrograms();
    for (int i = 0; i < fileNames.size(); i++)
    {
        if (i == 0)
        {
            compilerForEntrant0 = new Compiler();
            string error1;
            if (fileNames[0] != "" && ! compilerForEntrant0->compile(fileNames[0], error1) )
            {
                setError(fileNames[0] + " " + error1);
                return GWSTATUS_LEVEL_ERROR;
            }
            antNames[i] = compilerForEntrant0->getColonyName();
        }
        else if (i == 1)
        {
            compilerForEntrant1 = new Compiler();
            string error2;
            if (fileNames[1] != "" && ! compilerForEntrant1->compile(fileNames[1], error2) )
            {
                setError(fileNames[1] + " " + error2);
                return GWSTATUS_LEVEL_ERROR;
            }
            antNames[i] = compilerForEntrant1->getColonyName();

        }
        else if (i == 2)
        {
            compilerForEntrant2 = new Compiler();
            string error3;
            if ( fileNames[2] != "" && ! compilerForEntrant2->compile(fileNames[2], error3) )
            {
                setError(fileNames[2] + " " + error3);
                return GWSTATUS_LEVEL_ERROR;
            }
            antNames[i] = compilerForEntrant2->getColonyName();

        }
        else if (i == 3)
        {
            compilerForEntrant3 = new Compiler();
            string error4;
            if ( fileNames[3] != "" && ! compilerForEntrant3->compile(fileNames[3], error4) )
            {
                setError(fileNames[3] + " " + error4);
                return GWSTATUS_LEVEL_ERROR;
            }
            antNames[i] = compilerForEntrant3->getColonyName();

        }
    }
    
    m_tick = 0;
    m_field = new Field;
    std::string fieldFile = getFieldFilename();
    
    std::string error;
    if (m_field->loadField(fieldFile, error) != Field::LoadResult::load_success)
    {
        setError(fieldFile + " " + error);
        return GWSTATUS_LEVEL_ERROR; // something bad happened!
        
        // otherwise the load was successful and you can access the
        // contents of the field – here’s an example
    }
    
    Field::FieldItem item;
    Actor* ptr;
    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            item = m_field->getContentsOf(x, y);
            switch (item) {
                case Field::FieldItem::rock:
                    ptr = new Pebble(this, x, y);
                    m_list[y][x].push_back(ptr);
                    break;
               
                case Field::FieldItem::grasshopper:
                    ptr = new BabyGrasshopper(this, x, y);
                    m_list[y][x].push_back(ptr);
                    break;
                    
                case Field::FieldItem::food:
                    ptr = new Food(this, x, y, 6000);
                    m_list[y][x].push_back(ptr);
                    break;
                    
                case Field::FieldItem::poison:
                    ptr = new Poison(this, x, y);
                    m_list[y][x].push_back(ptr);
                    break;
                    
                case Field::FieldItem::water:
                    ptr = new WaterPool(this, x, y);
                    m_list[y][x].push_back(ptr);
                    break;
               
                case Field::FieldItem::anthill0:
                    if (compilerForEntrant0 != nullptr)
                    {
                        ah0 = new AntHill(this, x, y, 0, compilerForEntrant0);
                        m_list[y][x].push_back(ah0);
                    }
                    break;
                    
                case Field::FieldItem::anthill1:
                    if (compilerForEntrant1 != nullptr)
                    {
                        ah1 = new AntHill(this, x, y, 1, compilerForEntrant1);
                        m_list[y][x].push_back(ah1);
                    }
                    break;
                    
                case Field::FieldItem::anthill2:
                    if (compilerForEntrant2 != nullptr)
                    {
                        ah2 = new AntHill(this, x, y, 2, compilerForEntrant2);
                        m_list[y][x].push_back(ah2);
                    }
                    break;
                    
                case Field::FieldItem::anthill3:
                    if (compilerForEntrant3 != nullptr)
                    {
                        ah3 = new AntHill(this, x, y, 3, compilerForEntrant3);
                        m_list[y][x].push_back(ah3);
                    }
                    break;
                    
                default:
                    ptr = nullptr;
                    break;
            }
        }
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()  //iterate through your structure, telling each actor to doSomething()
{
    if (m_tick != 2000)
    {
        m_tick++;
        
        for (int y = 0; y < 64; y++)
        {
            for (int x = 0; x < 64; x++)
            {
                std::list<Actor*>::iterator it = m_list[y][x].begin();
                
                while(it != m_list[y][x].end())
                {
                    if (!m_list[y][x].empty())
                    {
                        (*it)->setoldX((*it)->getX());
                        (*it)->setoldY((*it)->getY());

                        (*it)->doSomething();
                    }
                    
                    it++;
                }
                
            }
        }
        
        for (int y = 0; y < 64; y++)
        {
            for (int x = 0; x < 64; x++)
            {
                std::list<Actor*>::iterator it = m_list[y][x].begin();
               
                while(it != m_list[y][x].end())
                {
                    if (!m_list[y][x].empty())
                    {
                        if ((*it)->isDead())
                        {
                            if ((*it)->isAnt())
                            {
                                setScore((*it)->getColony(), -1);
                            }
                            (*it)->setVisible(false);
                            m_list[y][x].erase(it);
                            it = m_list[y][x].begin();
                            
                        }
                        
                        else if( (*it)->getX() != (*it)->getoldX() || (*it)->getY() != (*it)->getoldY() )
                        {
                            int oldX = (*it)->getoldX();
                            int oldY = (*it)->getoldY();
                            int newX =  (*it)->getX();
                            int newY = (*it)->getY();
                            Actor* ptr_to_move = *it;

                            (*it)->setoldX((*it)->getX());
                            (*it)->setoldY((*it)->getY());
                            m_list[oldY][oldX].erase(it);
                            m_list[newY][newX].push_back(ptr_to_move);
                            it = m_list[y][x].begin();
                        }
                        else
                        {
                            it++;
                        }
                    }
                }
            }
        }
        
        setDisplayText();   //sets the display  //REDO THIS!
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
    vector<string> fileNames = getFilenamesOfAntPrograms();
    int size = fileNames.size();
    
    if (isWinnerCount(size) >= 6)
    {
        string winner = isWinner(size);
        setWinner(winner);
        return GWSTATUS_PLAYER_WON;
    }
    
    // REDO WINNER / NOWINNER SITUATION WITH COMPILER CLASS
    
    return GWSTATUS_NO_WINNER;
    
}

 //ptr_to_actor = actor_that_moved;
 //old_position_list.remove(actor_that_moved);
 //new_position_list.add(ptr_to_actor);
 

void StudentWorld::setDisplayText()
{
    int ticks = 2000 - m_tick;
    
    std::ostringstream oss;
    oss << "Ticks:" <<std::right << std::setw(5) << ticks;
    
    vector<string> fileNames = getFilenamesOfAntPrograms();
    int size = fileNames.size();
    
    
    string champ = isWinner(size);
    
    
    if (size == 1)
    {
        oss << " - " << antNames[0] << "*: " << setfill('0') << setw(2) << antCount[0];
    }
    else if (size == 2)
    {
        if (champ == antNames[0])
        {
            oss << " - " << antNames[0] << "*: " << setfill('0') << setw(2) << antCount[0] << "  " << antNames[1] << ": " << setfill('0') << setw(2) << antCount[1];
        }
        else
        {
            oss << " - " << antNames[0] << ": "  << setfill('0') << setw(2) << antCount[0] << "  " << antNames[1] << "*: "  << setfill('0') << setw(2) << antCount[1];
        }
    }
    //AmyAnt: 32 BillyAnt: 33 SuzieAnt*: 77 IgorAnt: 05
    else if (size == 3)
    {
        if (champ == antNames[0])
        {
            oss << " - " << antNames[0] << "*: "  << setfill('0') << setw(2) << antCount[0] << "  " << antNames[1] << ": "  << setfill('0') << setw(2) << antCount[1] << "  " << antNames[2] << ": "  << setfill('0') << setw(2) << antCount[2];
        }
        else if (champ == antNames[1])
        {
            oss << " - " << antNames[0] << ": "  << setfill('0') << setw(2) << antCount[0] << "  " << antNames[1] << "*: "  << setfill('0') << setw(2) << antCount[1] << "  " << antNames[2] << ": "  << setfill('0') << setw(2) << antCount[2];
        }
        else
        {
            oss << " - " << antNames[0] << ": "  << setfill('0') << setw(2) << antCount[0] << "  " << antNames[1] << ": "  << setfill('0') << setw(2) << antCount[1] << "  " << antNames[2] << "*: "  << setfill('0') << setw(2) << antCount[2];
        }
        
    }
    else if (size == 4)
    {
        if (champ == antNames[0])
        {
            oss << " - " << antNames[0] << "*: "  << setfill('0') << setw(2) << antCount[0] << "  " << antNames[1] << ": "  << setfill('0') << setw(2) << antCount[1] << "  " << antNames[2] << ": "  << setfill('0') << setw(2) << antCount[2] << "  " << antNames[3] << ": "  << setfill('0') << setw(2) << antCount[3];
        }
        else if (champ == antNames[1])
        {
            oss << " - " << antNames[0] << ": "  << setfill('0') << setw(2) << antCount[0] << "  " << antNames[1] << "*: "  << setfill('0') << setw(2) << antCount[1] << "  " << antNames[2] << ": "  << setfill('0') << setw(2) << antCount[2] << "  " << antNames[3] << ": "  << setfill('0') << setw(2) << antCount[3];
        }
        else if (champ == antNames[2])
        {
            oss << " - " << antNames[0] << ": "  << setfill('0') << setw(2) << antCount[0] << "  " << antNames[1] << "*: "  << setfill('0') << setw(2) << antCount[1] << "  " << antNames[2] << ": "  << setfill('0') << setw(2) << antCount[2] << "  " << antNames[3] << ": "  << setfill('0') << setw(2) << antCount[3];
        }
        else
        {
            oss << " - " << antNames[0] << ": "  << setfill('0') << setw(2) << antCount[0] << "  " << antNames[1] << ": "  << setfill('0') << setw(2) << antCount[1] << "  " << antNames[2] << ": "  << setfill('0') << setw(2) << antCount[2] << "  " << antNames[3] << "*: "  << setfill('0') << setw(2) << antCount[3];
        }
    }
    
    std::string s = oss.str();
    setGameStatText(s); // calls our provided GameWorld::setGameStatText


}

string StudentWorld::isWinner(int size)
{
    int winner = 0;
    string realWinner;
    for (int i = 0; i < size; i++)
    {
        if (antCount[i] > winner)
        {
            winner = antCount[i];
            realWinner = antNames[i];
        }
    }
    return realWinner;
}

int StudentWorld::isWinnerCount(int size)
{
    int winner = 0;
    for (int i = 0; i < size; i++)
    {
        if (winner < antCount[i])
            winner = antCount[i];
    }
    return winner;
}

void StudentWorld::cleanUp()
{
    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            std::list<Actor*>::iterator it = m_list[y][x].begin();
            if (!m_list[y][x].empty())
            {
                m_list[y][x].erase(it);
            }
        }
    }

}

// Can an insect move to x,y?
bool StudentWorld::canMoveTo(int x, int y) const
{
    Field::FieldItem item;
    item = m_field->getContentsOf(x, y);
    if (item == Field::FieldItem::rock)
        return true;
    else
        return false;
}


Field* StudentWorld::get_Field()
{
    return m_field;
}

void StudentWorld::addActor(Actor *a)
{
    m_list[a->getY()][a->getX()].push_back(a);
}



// If an item that can be picked up to be eaten is at x,y, return a
// pointer to it; otherwise, return a null pointer.  (Edible items are
// only ever going be food.)
Actor* StudentWorld::getEdibleAt(int x, int y) const
{
    std::list<Actor*>::const_iterator it = m_list[y][x].begin();
    
    while(it != m_list[y][x].end())
    {
        if (!m_list[y][x].empty())
        {
            if ((*it)->isEdible())
            {
                return (*it);
            }
            else
            {
                it++;
            }
        }
    }
    return nullptr;
}

// If a pheromone of the indicated colony is at x,y, return a pointer
// to it; otherwise, return a null pointer.
Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
{
    std::list<Actor*>::const_iterator it = m_list[y][x].begin();
    
    while(it != m_list[y][x].end())
    {
        if (!m_list[y][x].empty())
        {
            if ((*it)->isPheromone(colony))
            {
                return (*it);
            }
            else
            {
                it++;
            }
        }
    }
    return nullptr;
}

// Is an enemy of an ant of the indicated colony at x,y?
bool StudentWorld::isEnemyAt(int x, int y, int colony, Actor* ptr)
{
    std::list<Actor*>::const_iterator it = m_list[y][x].begin();

    while(it != m_list[y][x].end())
    {
        if (!m_list[y][x].empty())
        {
            if ((*it) != ptr && ((*it)->getEnergy()) > 0 && (*it)->isEnemy(ptr->getColony()))
            {
                return true;
            }
            else
            {
                it++;
            }
        }
    }

    return false;
}

// Is something dangerous to an ant of the indicated colony at x,y?
bool StudentWorld::isDangerAt(int x, int y, int colony, Actor* ptr) const
{
    std::list<Actor*>::const_iterator it = m_list[y][x].begin();
    
    while(it != m_list[y][x].end())
    {
        if (!m_list[y][x].empty())
        {
            if (((*it) != ptr && ((*it)->getEnergy()) > 0 && (*it)->isEnemy(ptr->getColony())) || (*it)->isDangerous())
            {
                return true;
            }
            else
            {
                it++;
            }
        }
    }
    
    return false;

}

bool StudentWorld::biteEnemy(Actor* me, int biteDamage)
{
    int y = me->getY();
    int x = me->getX();
    
    std::vector<Actor*> enemy;
    std::list<Actor*>::const_iterator it = m_list[y][x].begin();
    
    while(it != m_list[y][x].end())
    {
        if (!m_list[y][x].empty())
        {
            if ((*it) != me && ((*it)->getEnergy()) > 0 && (*it)->isEnemy(me->getColony()))
            {
                enemy.push_back((*it));
                it++;
            }
            else
            {
                it++;
            }
        }
    }
    if (enemy.size() > 0)
    {
        int random = randInt(0, enemy.size()-1);
        enemy[random]->updateEnergy(-biteDamage);
        if (enemy[random]->isAnt())
        {
            enemy[random]->updateBitten(true);
        }
        if (!enemy[random]->isDead() && enemy[random]->isAdult() && (randInt(0, 1) == 0))
        {
            biteEnemy(enemy[random], 50);
        }
        return true;
            
    }
    return false;

}

bool StudentWorld::myHill(int x, int y, int colony)
{
    std::list<Actor*>::const_iterator it = m_list[y][x].begin();
    
    while(it != m_list[y][x].end())
    {
        if (!m_list[y][x].empty())
        {
            if (!(*it)->isDead() && (*it)->isMyHill(colony))
            {
                return true;
            }
            else
            {
                it++;
            }
        }
        
    }
    return false;
}

bool StudentWorld::poisonAllPoisonableAt(int x, int y)
{
    std::list<Actor*>::const_iterator it = m_list[y][x].begin();
    
    while(it != m_list[y][x].end())
    {
        if (!m_list[y][x].empty())
        {
            if ((*it)->isInsect() && (!(*it)->isPoisoned()))
            {
                (*it)->getPoisoned();
            }
            else
            {
                it++;
            }
        }
        
    }
    return true;
}

bool StudentWorld::stunAllStunnableAt(int x, int y)
{
    std::list<Actor*>::const_iterator it = m_list[y][x].begin();
    
    while(it != m_list[y][x].end())
    {
        if (!m_list[y][x].empty())
        {
            if ((*it)->isInsect() && (!(*it)->isStunned()))
            {
                (*it)->getStunned();
            }
            else
            {
                it++;
            }
        }
        
    }
    return true;
}

// Record another ant birth for the indicated colony.
void StudentWorld::setScore(int colony, int amt)
{
    antCount[colony] += amt;
}
