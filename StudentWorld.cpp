#include "StudentWorld.h"
#include <string>
#include "GraphObject.h"
#include <sstream>  // defines the type std::ostringstream
#include <iostream> // defines the overloads of the << operator
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{
	int loadedLevel = loadLevel();
	if (loadedLevel == -1)			// error if level is formatted incorrectly
	{
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (loadedLevel == 1)		// player won if file not found
	{
		return GWSTATUS_PLAYER_WON;
	}
	else if (loadedLevel == 2)		// player won if reach level 100
	{
		return GWSTATUS_PLAYER_WON;
	}
	return GWSTATUS_CONTINUE_GAME;
}

StudentWorld::~StudentWorld()
{
	list<Actor*>::iterator it;			// deletes all dynamically allocated memory using iterator through list
	it = m_actors.begin();
	while (it != m_actors.end())
	{
		delete *it;
		it++;
	}
	m_actors.clear();					// clears the list
}
int	StudentWorld::loadLevel()
{
	int level = getLevel();
	if (level == 100)		// check for level 100 reach
		return 2;
	string curLevel;
	ostringstream oss;				// create ostringstream that converts level to dat file string
	oss.fill('0');		
	oss << "level" << setw(2) << level << ".dat";		
	curLevel = oss.str();						
	Level	lev(assetDirectory());
	Level::LoadResult result = lev.loadLevel(curLevel);
	if (result == Level::load_fail_bad_format)
		return -1; //	something	bad	happened!
	if (result == Level::load_fail_file_not_found)
		return 1;
	for (int x = 0; x < VIEW_WIDTH; x++)
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
			Level::MazeEntry item = lev.getContentsOf(x, y);
			if (item == Level::player)				// adds player to back of list
			{
				m_player = new Player(x, y, this);
				m_actors.push_back(m_player);
			}
			if (item == Level::wall)				// adds wall to back of list
			{
				m_actors.push_back(new Wall(x, y, this));
			}
			if (item == Level::boulder)				// adds boulder to back of list
			{
				m_actors.push_back(new Boulder(this, x, y));
			}
			if (item == Level::exit)				// adds exit to back of list
			{
				m_actors.push_back(new Exit(this, x, y));
			}
			if (item == Level::jewel)				// adds jewel to front of list
			{
				m_actors.push_front(new Jewel(this, x, y));
				m_jewels++;
			}
			if (item == Level::extra_life)			// adds extra life pack to front of list
			{
				m_actors.push_front(new ExtraLifeGoodie(this, x, y));
			}
			if (item == Level::restore_health)		// adds health pack to front of list
			{
				m_actors.push_front(new RestoreHealthGoodie(this, x, y));
			}
			if (item == Level::ammo)				// adds ammo to front of list
			{
				m_actors.push_front(new AmmoGoodie(this, x, y));
			}
			if (item == Level::hole)				// adds hole to front of list
			{
				m_actors.push_front(new Hole(this, x, y));
			}
			if (item == Level::horiz_snarlbot)		// adds horizontal snarlbot to back of list
			{
				m_actors.push_back(new SnarlBot(this, x, y, GraphObject::right));
			}
			if (item == Level::vert_snarlbot)		// adds vertical snarlbot to back of list
			{
				m_actors.push_back(new SnarlBot(this, x, y, GraphObject::down));
			}
			if (item == Level::kleptobot_factory)	// adds kleptobot factory to front of list
			{
				m_actors.push_front(new KleptoBotFactory(this, x, y, KleptoBotFactory::REGULAR));
			}
			if (item == Level::angry_kleptobot_factory)	// adds angry kleptobot factory to front of list
			{
				m_actors.push_front(new KleptoBotFactory(this, x, y, KleptoBotFactory::ANGRY));
			}
		}
	}
	return 0;
}

// Restore player's health to the full amount.
void StudentWorld::restorePlayerHealth()
{
	getPlayer()->restoreHealth();
}

// Increase the amount of ammunition the player has
void StudentWorld::increaseAmmo()
{
	getPlayer()->increaseAmmo();
}
string StudentWorld::someFunctionToFormatThingsNicely(int score, int level, int lives, int health, int ammo, int bonus)
{
	ostringstream oss;
	oss.fill('0');
	oss << "Score: " << setw(7) << score << "  ";
	oss << "Level: " << setw(2) << level << "  ";
	oss.fill(' ');
	oss << "Lives: " << setw(2) << lives << "  ";
	oss << "Health: " << setw(3) << health << "%  ";
	oss << "Ammo: " << setw(3) << ammo << "  ";
	oss << "Bonus: " << setw(4) << bonus;
	string s = oss.str();
	return s;
}
void StudentWorld::setDisplayText()
{
	int score = getScore();
	int level = getLevel();
	unsigned int bonus = getCurrentLevelBonus();
	int livesLeft = getLives();
	int health = m_player->getHealthPct();
	int ammo = m_player->getAmmo();
	// Next, create a string from your statistics, of the form:
	// Score: 0000100 Level: 03 Lives: 3 Health: 70% Ammo: 216 Bonus: 34
	string s = someFunctionToFormatThingsNicely(score, level, livesLeft,
		health, ammo, bonus);
	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

int StudentWorld::move()
{
	setDisplayText();
	if (getCurrentLevelBonus() != 0)		// decrement bonus by one each tick until bonus is 0
		m_bonus--;
	list<Actor*>::iterator it;
	list<Actor*>::iterator holder;
	it = m_actors.begin();
	while (it != m_actors.end())			// traverse through list and make every actor call their doSomething function
	{
		(*it)->doSomething();
		if (!(getPlayer()->isAlive()))		// stop level if player has died and decrement lives
		{
			playSound(SOUND_PLAYER_DIE);
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		if (level_finished)					// increase score by exit score and bonus
		{
			increaseScore(2000);
			increaseScore(m_bonus);
			m_bonus = 1000;					// reset bonus for next level set level_finished to false and go to next level
			level_finished = false;
			playSound(SOUND_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}
		if ((*it)->isAlive() == false)			// kill all the dead actors
		{
			delete *it;
			holder = m_actors.erase(it);		
			it = holder;
		}
		else
			it++;
	}

	return GWSTATUS_CONTINUE_GAME;
}

Actor* StudentWorld::isActor(int x, int y)
{
	list<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end())		// go through list from beginning and return first actor that is at position x, y
	{
		if ((*it)->getX() == x && (*it)->getY() == y)
			return (*it);
		it++;
	}
	return nullptr;
}

Actor* StudentWorld::isActorBackwards(int x, int y)
{
	list<Actor*>::iterator it;
	it = m_actors.end();
	while (it != m_actors.begin())		// go through list starting from the end and return first actor that is at position x, y
	{
		it--;
		if ((*it)->getX() == x && (*it)->getY() == y)
			return (*it);
	}
	return nullptr;
}

void StudentWorld::cleanUp()
{
	list<Actor*>::iterator it;			// delete dynamically allocated array and clear list
	it = m_actors.begin();
	while (it != m_actors.end())
	{
		delete *it;
		it++;
	}
	m_actors.clear();
}

// Is the player on the same square as an Actor?
bool StudentWorld::isPlayerColocatedWith(int x, int y) const
{
	if ((getPlayer()->getX()) == x && (getPlayer()->getY()) == y)
		return true;
	return false;
}

// Can agent move to x,y?  (dx and dy indicate the direction of motion)
bool StudentWorld::canAgentMoveTo(Agent* agent, int x, int y, int dx, int dy) 
{
	Actor* a = isActorBackwards(x, y);
	if (a == nullptr)
		return true;
	else if (a->bePushedBy(agent, x + dx, y + dy)) // check for boulder
	{
		return true;
	}
	else if (a->allowsAgentColocation())			// check to see if multiple actors can occupy same place
		return true;
	return false;
}

// Can a boulder move to x,y?
bool StudentWorld::canBoulderMoveTo(int x, int y) 
{
	Actor* a = isActor(x, y);
	if (a == nullptr)					// boulder can be moved onto empty space
		return true;
	else if (a->allowsBoulderColocation())		// check to see if hole
		return true;
	return false;
}

void StudentWorld::addBullet(Actor* a)
{
	int dx = 0;
	int dy = 0;
	int dir = a->getDirection();
	switch (dir)			// adjust dx or dy accordingly based on direction in order to create new bullet
	{
	case GraphObject::up:
		dy++;
		break;
	case GraphObject::down:
		dy--;
		break;
	case GraphObject::left:
		dx--;
		break;
	case GraphObject::right:
		dx++;
		break;
	}
	m_actors.push_front(new Bullet(this, a->getX() + dx, a->getY() + dy, a->getDirection()));
}

void StudentWorld::addKleptoBot(KleptoBotFactory* a)
{
	if (a->getProductType() == KleptoBotFactory::REGULAR)							// sets the kleptobot on top of the factory
		m_actors.push_back(new RegularKleptoBot(this, a->getX(), a->getY()));
	else if (a->getProductType() == KleptoBotFactory::ANGRY)
		m_actors.push_back(new AngryKleptoBot(this, a->getX(), a->getY()));
}