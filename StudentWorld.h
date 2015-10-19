#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <list>
#include "Actor.h"
#include	"Level.h" //	you	must	include	this	file	to	use	our	Level	class
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir), m_bonus(1000), level_finished(false)
	{
	}

	virtual ~StudentWorld();

	int	StudentWorld::loadLevel();

	virtual int init();

	virtual int move();

	Actor* isActor(int x, int y);		// goes through list from beginning and returns first instance of actor at position x,y
	Actor* isActorBackwards(int x, int y);	// goes through list from end and returns first instance of actor at position x,y



	void setDisplayText();

	virtual void cleanUp();
	// Can agent move to x,y?  (dx and dy indicate the direction of motion)
	bool canAgentMoveTo(Agent* agent, int x, int y, int dx, int dy);

	// Can a boulder move to x,y?
	bool canBoulderMoveTo(int x, int y);

	// Is the player on the same square as an Actor?
	bool isPlayerColocatedWith(int x, int y) const;

	// Restore player's health to the full amount.
	void restorePlayerHealth();

	// Increase the amount of ammunition the player has
	void increaseAmmo();

	// Are there any jewels left on this level?
	bool anyJewels() const { return m_jewels != 0; }

	// Reduce the count of jewels on this level by 1.
	void decJewels() { m_jewels--; }

	// Indicate that the player has finished the level.
	void setLevelFinished() { level_finished = true; };

	// Add an bullet to the world
	void addBullet(Actor* a);

	// return current level bonus
	int getCurrentLevelBonus() const { return m_bonus; }

	// return pointer to the player
	Player* getPlayer() const { return m_player; }

	// add kleptobot from the kleptobotfactory
	void addKleptoBot(KleptoBotFactory* a);

	//formats the display text nicely
	string someFunctionToFormatThingsNicely(int score, int level, int lives, int health, int ammo, int bonus);
private:
	list<Actor*> m_actors;
	int m_bonus;
	Player* m_player;
	bool level_finished;
	int m_jewels;
};

#endif // STUDENTWORLD_H_
