#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
class Agent;
class Goodie;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, StudentWorld* world, unsigned int hitPoints = 0, Direction dir = none);
	virtual ~Actor(){}
	// abstract class that all actors need which describes what they do on each tick
	virtual void doSomething() = 0;

	// returns the StudentWorld that the actor belongs to
	StudentWorld* getWorld() const { return m_world; }

	// returns the actor pointer at the spot x,y and nullptr if empty. Sidenote: goes through list from beginning
	Actor* getActorAtLocation(int x, int y) const;

	// does the actor block the player?
	virtual bool blocksPlayer() const { return false; }
	// Is this actor alive?
	bool isAlive() const { return m_alive; }
	// Mark this actor as dead
	void setDead() { m_alive = false; }

	// returns the actor pointer at the spot x,y and nullptr if empty. Sidenote: goes through list from end
	Actor* Actor::getActorAtLocationBackwards(int x, int y) const;

	// sets bool value in goodie as stolen
	virtual void isStolen() { return; }		
	// sets bool value in goodie as not stolen
	virtual void notStolen() { return; }
	// returns the score that will be given to the player
	virtual int getScore() { return 0; }

	// Return true if this agent can push boulders(which means it's the
	// player).
	virtual bool canPushBoulders() const { return false; }

	// uses direction to alter dx and dy values
	void changeInLocation(Actor* a, int& dx, int& dy);

	// Can an agent occupy the same square as this actor?			
	virtual bool allowsAgentColocation() const { return false; }
	// Can a boulder occupy the same square as this actor?
	virtual bool allowsBoulderColocation() const { return false; }
	// Does this actor count when a factory counts items near it?
	virtual bool countsInFactoryCensus() const { return false; }

	// Does this actor stop bullets from continuing?
	virtual bool stopsBullet() const { return false; }

	// Can this actor be damaged by bullets?
	virtual bool isDamageable() const { return false; }

	// Cause this Actor to sustain damageAmt hit points of damage.
	virtual void damage(int damageAmt) { m_hp -= damageAmt; }

	// Can this actor be pushed by a to location x,y?
	virtual bool bePushedBy(Agent* a, int x, int y) { return false; }

	// Can this actor be swallowed by a hole?
	virtual bool isSwallowable() const { return false; }

	// Can this actor be picked up by a KleptoBot?
	virtual bool isStealable() const { return false; }				

	// How many hit points does this actor have left?  
	int getHitPoints() const { return m_hp; }

	// Set this actor's hit points to amt.
	void setHitPoints(int amt){ m_hp = amt; }

	// Make the actor sustain damage.  Return true if this kills the
	// actor, and false otherwise.
	bool tryToBeKilled(int damageAmt);

private:
	StudentWorld* m_world;
	bool m_alive;
	int m_hp;
};

class Agent : public Actor
{
public:
	Agent(int imageID, int startX, int startY, StudentWorld* world, Direction dir, unsigned int hitPoints);
	virtual ~Agent(){}
	// Move to the adjacent square in the direction the agent is facing
	// if it is not blocked, and return true.  Return false if the agent
	// can't move.
	bool moveIfPossible();

	// agents are able to take damage 
	virtual bool isDamageable() const { return true; }

	// Return true if this agent can push boulders (which means it's the
	// player).
	virtual bool needsClearShot() const;

	//agents stop bullets from moving
	virtual bool stopsBullet() const { return true; }
	// agents block player from moving
	virtual bool blocksPlayer() const { return true; }
};

class Robot : public Agent
{
public:
	Robot(StudentWorld* world, int startX, int startY, int imageID,
		unsigned int hitPoints, unsigned int score, Direction startDir);
	virtual ~Robot(){}
	// robots are unable to push boulders
	virtual bool canPushBoulders() const { return false; }
	// used to determine whether robot is allowed to shoot
	virtual bool needsClearShot();
	
	// used to play sound when damaged
	virtual bool isDamageable() const;

	//checks to see whether it can move due to tick system
	bool isItTime();

	// used to play death sound when dead
	virtual void damage(int damageAmt);

	// used to obtain private data member m_score
	virtual int getScore() { return m_score; }
private:
	int m_score;
	int currentTick;		
	int m_ticks;
	bool timeToAct;
};

class SnarlBot : public Robot
{
public:
	SnarlBot(StudentWorld* world, int startX, int startY, Direction startDir);
	virtual ~SnarlBot(){}
	virtual void doSomething(); 
};

class KleptoBot : public Robot
{
public:
	KleptoBot(StudentWorld* world, int startX, int startY, int imageID,
		unsigned int hitPoints, unsigned int score);
	virtual ~KleptoBot(){}
	
	// used when factory does its census
	virtual bool countsInFactoryCensus() const { return true; }

	// increment turns tick
	void incTurn(){ m_turns++; }

	// turns left before can act
	int turnsLeft() { return m_turns; }

	// return private data member m_distanceBeforeTurning
	int distanceBeforeTurning() { return m_distanceBeforeTurning; }

	// create new random distance before turning
	void setNewDistanceBeforeTurning(int a) { m_distanceBeforeTurning = a; }

	// sets a random direction
	void setRandomDirection(int times);

	// sets m_turns back to 0 to restart cycle
	void resetTurns() { m_turns = 0; }

	// retrieves pointer to private data member Goodie since each KleptoBot may have a Goodie
	Goodie* myGoodie() { return m_goodie; }

	// set Kleptobot m_goodie value
	void setGoodie(Goodie* a) { m_goodie = a; }
private:
	Goodie* m_goodie;
	int m_turns;
	int m_distanceBeforeTurning;
};

class RegularKleptoBot : public KleptoBot
{
public:
	RegularKleptoBot(StudentWorld* world, int startX, int startY);
	virtual ~RegularKleptoBot(){}

	// has its own original behavior
	virtual void doSomething();

	// special case since RegularKleptoBot can carry goodie 
	virtual void damage(int damageAmt);
};

class AngryKleptoBot : public KleptoBot
{
public:
	AngryKleptoBot(StudentWorld* world, int startX, int startY);
	virtual ~AngryKleptoBot(){}

	// has RegularKleptoBot's but also tries to shoot
	virtual void doSomething();

	// used to place goodies down if dead
	virtual void damage(int damageAmt);
};

class KleptoBotFactory : public Actor
{
public:
	enum ProductType { REGULAR, ANGRY };

	KleptoBotFactory(StudentWorld* world, int startX, int startY, ProductType type);
	virtual ~KleptoBotFactory() {}

	// create KleptoBots
	virtual void doSomething();

	// blocks bullets from continuing
	virtual bool stopsBullet() const { return true; }

	// returns private data member m_productType
	ProductType getProductType() { return m_productType; }

	// does not allow to re-step onto factory
	virtual bool allowsAgentColocation() const { return false; }
private:
	int m_count;
	ProductType m_productType;
};

class Bullet : public Actor
{
public:
	Bullet(StudentWorld* world, int startX, int startY, Direction startDir);
	virtual ~Bullet(){}

	// special behavior for bullet
	virtual void doSomething();

	// allows actors to be on same space
	virtual bool allowsAgentColocation() const { return true; }
};
class Boulder : public Actor
{
public:
	Boulder(StudentWorld* world, int startX, int startY);
	virtual ~Boulder(){}
	// does nothing each tick
	virtual void doSomething() { return; }

	// is able to take damage and blow up
	virtual bool isDamageable() const { return true; }
	
	// is able to be swallowed by hole
	virtual bool isSwallowable() const { return true; }

	// deals with hole and boulder interaction
	virtual bool bePushedBy(Agent* a, int x, int y);

	// blocks Player from moving
	virtual bool blocksPlayer() const { return true; }

	// blocks bullets from continuing
	virtual bool stopsBullet() const { return true; }
};

class Hole : public Actor
{
public:
	Hole(StudentWorld* world, int startX, int startY);
	virtual ~Hole(){}
	// does nothing
	virtual void doSomething() { return; }
	
	// allows boulder to be on same space
	virtual bool allowsBoulderColocation() const { return true; }
	
	// prevents player from moving onto hole
	virtual bool blocksPlayer() const { return true; }
};
class Wall : public Actor
{
public:
	Wall(int startX, int startY, StudentWorld* world);
	virtual ~Wall(){}
	// does nothing
	virtual void doSomething() { return; }

	// prevents player from moving
	virtual bool blocksPlayer() const { return true; }

	//prevents bullets from moving
	virtual bool stopsBullet() const { return true; }

};
class Player : public Agent
{
public:
	Player(int startX, int startY, StudentWorld* world);
	virtual ~Player(){}
	// takes care of interaction with keyboard to move
	virtual void doSomething();

	//is able to push boulders
	virtual bool canPushBoulders() const { return true; }

	// does not allow for other agents to take same space
	virtual bool allowsAgentColocation() const { return false; }

	// Get player's health percentage
	unsigned int getHealthPct() const { return (getHitPoints() * 5); }

	// Get player's amount of ammunition
	unsigned int getAmmo() const { return m_ammo; }

	// Restore player's health to the full amount.
	void restoreHealth() { setHitPoints(20); }

	// Increase player's amount of ammunition.
	void increaseAmmo() { m_ammo += 20; }

private:
	int m_ammo;
};

class Exit : public Actor
{
public:
	Exit(StudentWorld* world, int startX, int startY);
	virtual ~Exit(){}
	// Can an agent occupy the same square as this actor?				
	virtual bool allowsAgentColocation() const { return true; }

	// takes care of special interaction with number of jewels
	virtual void doSomething();

	// does not prevent player from moving
	virtual bool blocksPlayer() const { return false; }
private:
	bool makeSoundEffect;
};

class PickupableItem : public Actor
{
public:
	PickupableItem(StudentWorld* world, int startX, int startY, int imageID,
		unsigned int score);
	virtual ~PickupableItem() {}

	// checks to see if player is on top of PickupableItem
	bool checkForPlayer();

	// Can an agent occupy the same square as this actor?				
	virtual bool allowsAgentColocation() const { return true; }
	
	// created pure virtual since all classes derived from PickupableItem need a doSomething implementation
	virtual void doSomething() = 0;

	// does not block player from moving
	virtual bool blocksPlayer() const { return false; }

	// return private data member m_score
	int score() const { return m_score; }
private:
	int m_score;
};

class Jewel : public PickupableItem
{
public:
	Jewel(StudentWorld* world, int startX, int startY);
	virtual ~Jewel(){}
	//decrease jewel count if player lands on one
	virtual void doSomething();
};

class Goodie : public PickupableItem
{
public:
	Goodie(StudentWorld* world, int startX, int startY, int imageID, int score);
	virtual ~Goodie() {}
	// returns whether or not goodie is stealable by kleptobot
	virtual bool isStealable() const;

	virtual void isStolen() { m_stolen = true; }
	virtual void notStolen() { m_stolen = false; }

	// return private member value m_stolen
	bool stolen() { return m_stolen; }
private:
	bool m_stolen;
};

class ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(StudentWorld* world, int startX, int startY);
	virtual ~ExtraLifeGoodie() {}
	// player gains a life
	virtual void doSomething();
};

class RestoreHealthGoodie : public Goodie
{
public:
	RestoreHealthGoodie(StudentWorld* world, int startX, int startY);
	virtual ~RestoreHealthGoodie() {}
	// restore player to full health
	virtual void doSomething();
};

class AmmoGoodie : public Goodie
{
public:
	AmmoGoodie(StudentWorld* world, int startX, int startY);
	virtual ~AmmoGoodie() {}
	// player gains 20 ammo
	virtual void doSomething();
};

#endif // ACTOR_H_
