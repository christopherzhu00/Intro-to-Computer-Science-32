#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR 

Actor::Actor(int imageID, int startX, int startY, StudentWorld* world, unsigned int hitPoints, Direction dir) : GraphObject(imageID, startX, startY, dir), m_world(world) 
{
	m_hp = hitPoints;
	setVisible(true);
	m_alive = true;
}

Actor* Actor::getActorAtLocation(int x, int y) const
{
	if ((x < 0 || y < 0) || (x >= VIEW_WIDTH || y >= VIEW_HEIGHT))	// return nullptr if out of bounds
		return nullptr;
	return m_world->isActor(x, y);	// goes through list and returns actor at location x, y

}

Actor* Actor::getActorAtLocationBackwards(int x, int y) const
{
	if ((x < 0 || y < 0) || (x >= VIEW_WIDTH || y >= VIEW_HEIGHT))	// return nullptr if out of bounds
		return nullptr;
	return m_world->isActorBackwards(x, y);		// goes through list from the end and returns actor at location x, y

}



void Actor::changeInLocation(Actor* a, int& dx, int& dy)
{
	int dir = a->getDirection();			// changes dx or dy accordingly based on direction
	dx = 0;
	dy = 0;
	switch (dir)
	{
	case up:
		dy++;
		break;
	case down:
		dy--;
		break;
	case left:
		dx--;
		break;
	case right:
		dx++;
		break;
	}
}
bool Actor::tryToBeKilled(int damageAmt)
{
	damage(damageAmt);			// take damage
	if (canPushBoulders())		// player gets hit
	{
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
	}
	if (m_hp <= 0)				// player has died
	{
		if (canPushBoulders())
		{
			getWorld()->playSound(SOUND_PLAYER_DIE);
		}
		
		return true;
	}
	return false;
}

// BULLET

Bullet::Bullet(StudentWorld* world, int startX, int startY, Direction startDir) : Actor(IID_BULLET, startX, startY, world, 1, startDir)
{
}

void Bullet::doSomething()
{
	if (!isAlive())			// return if not alive
		return;
	Actor* a = getActorAtLocationBackwards(getX(), getY());		// checks current location of bullet, check backwards since robots are placed at the end
	if (a != nullptr)
	{
		if (a->stopsBullet())
		{
			if (a->isDamageable())
			{
				if (a->tryToBeKilled(2))
				{
					a->setDead();
					getWorld()->increaseScore(a->getScore());
				}
			}
			setDead();
			return;
		}
	}
	int dx, dy = 0;
	changeInLocation(this, dx, dy);					// checks the next location that the bullet is going to be
	moveTo(getX() + dx, getY() + dy);
	Actor* b = getActorAtLocationBackwards(getX(), getY());
	if (b != nullptr)
	{
		if (b->stopsBullet())
		{
			if (b->isDamageable())
			{
				if (b->tryToBeKilled(2))
				{
					b->setDead();
					getWorld()->increaseScore(b->getScore());
				}
			}
		setDead();
		return;
		}
	}
}

// AGENT

Agent::Agent(int imageID, int startX, int startY, StudentWorld* world, Direction dir, unsigned int hitPoints) : Actor(imageID, startX, startY, world, hitPoints, dir)
{
}
// Move to the adjacent square in the direction the agent is facing
// if it is not blocked, and return true.  Return false if the agent
// can't move.

bool Agent::moveIfPossible()
{
	int dx, dy = 0;
	changeInLocation(this, dx, dy);		// sets up values of dx and dy based on direction
	if (getWorld()->canAgentMoveTo(this, getX() + dx, getY() + dy, dx, dy))		// checks to see if can move to new location
	{
		moveTo(getX() + dx, getY() + dy);			// moves to new location
		return true;
	}
	return false;
}


bool Agent::needsClearShot() const
{
	return true;
}



// ROBOT

Robot::Robot(StudentWorld* world, int startX, int startY, int imageID,
	unsigned int hitPoints, unsigned int score, Direction startDir) : Agent(imageID, startX, startY, world, startDir, hitPoints), m_score(score)
{
	m_score = score;
	currentTick = 0;
	m_ticks = (28 - getWorld()->getLevel()) / 4; // levelNumber is the current
	// level number (0, 1, 2, etc.)
	if (m_ticks < 3)
		m_ticks = 3; // no SnarlBot moves more frequently than this
}

bool Robot::isDamageable() const
{
	getWorld()->playSound(SOUND_ROBOT_IMPACT);		// plays sound when damaged
	return true;
}
bool Robot::isItTime()					
{
	currentTick++;
	if (currentTick % m_ticks == 0)			// increments tick and checks to see if it is its turn to move
		return true;
	return false;
}
bool Robot::needsClearShot() 
{
	int dx, dy = 0;
	int xPos = getX();
	int yPos = getY();
	changeInLocation(this, dx, dy);		// sets up values for dx, dy based on direction
	Actor* a;
	do
	{	
		xPos += dx;
		yPos += dy;
		a = getActorAtLocationBackwards(xPos, yPos);	// looks through list backwards since most blockers are pushed back
		if (a == nullptr)
			continue;
		if (a->canPushBoulders())						// checks for player
			return true;
		else if (a->stopsBullet())						// checks for blocker
			return false;
	} while (true);
	
}

void Robot::damage(int damageAmt)
{
	Actor::damage(damageAmt);							// plays robot die sound if dead
	if (getHitPoints() <= 0)
		getWorld()->playSound(SOUND_ROBOT_DIE);
}

// SNARLBOT

SnarlBot::SnarlBot(StudentWorld* world, int startX, int startY, Direction startDir) : Robot(world, startX, startY, IID_SNARLBOT, 10, 100, startDir)
{
}

void SnarlBot::doSomething()
{
	if (isItTime())										// checks to see if it can act
	{
		if (needsClearShot())							// fires bullet if clear shot is available
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			getWorld()->addBullet(this);
		}
		else if (!(moveIfPossible()))					// tries to move but can not so reverses direction and tries to shoot + move
		{
			if (getDirection() == right)				
			{
				setDirection(left);
				if (!needsClearShot())
					moveIfPossible();
			}
			else if (getDirection() == left)
			{
				setDirection(right);
				if (!needsClearShot())
					moveIfPossible();
			}
			else if (getDirection() == up)
			{
				setDirection(down);
				if (!needsClearShot())
					moveIfPossible();
			}
			else if (getDirection() == down)
			{
				setDirection(up);
				if (!needsClearShot())
					moveIfPossible();
			}
		}
	}
}

// KLEPTOBOT

KleptoBot::KleptoBot(StudentWorld* world, int startX, int startY, int imageID,
	unsigned int hitPoints, unsigned int score) : Robot(world, startX, startY, imageID, hitPoints, score, right)
{
	m_distanceBeforeTurning = (rand() % 6) + 1;
	m_goodie = nullptr;
	m_turns = 0;
}

void KleptoBot::setRandomDirection(int times)
{
	int i = rand() % times;			
	switch (i)
	{
	case 1:
		setDirection(left);
		break;
	case 2:
		setDirection(right);
		break;
	case 3:
		setDirection(up);
		break;
	case 4:
		setDirection(down);
		break;
	}
}
// REGULAR KLEPTOBOT

RegularKleptoBot::RegularKleptoBot(StudentWorld* world, int startX, int startY) : KleptoBot(world, startX, startY, IID_KLEPTOBOT, 5, 10)
{
}

void RegularKleptoBot::doSomething()
{
	if (!isAlive())
		return;
	if (isItTime())
	{
		Actor* a = getActorAtLocation(getX(), getY());			// check list from beginning since goodies are placed in front
		if (a->isStealable() && myGoodie() == nullptr)			// only steal if does not have a goodie and the actor is a goodie
		{
			if ((rand() % 10) == 1)								// random chance to steal
			{
				setGoodie(dynamic_cast<Goodie*>(a));			
				a->isStolen();									//makes it so that goodie doesn't do anything anymore while its stolen
				getWorld()->playSound(SOUND_ROBOT_MUNCH);		
				a->setVisible(false);							// hides the goodie
				return;
			}
		}
		if (turnsLeft() < distanceBeforeTurning() && moveIfPossible())	// try to move if there are still turns left
		{
			incTurn();
		}
		else
		{
			setNewDistanceBeforeTurning((rand() % 6) + 1);				// set new random distance and reset values
			resetTurns();

			for (int i = 4; i > 0; i--)									// keep setting random direction until it can move
			{
				setRandomDirection(i);
				if (moveIfPossible())
				{
					incTurn();
					break;
				}
			}

		}
	}
}

void RegularKleptoBot::damage(int damageAmt)
{
	Robot::damage(damageAmt);
	if (getHitPoints() <= 0)						// drop goodie if stole at current location when dead
	{
		if (myGoodie() != nullptr)
		{
			myGoodie()->setVisible(true);
			myGoodie()->notStolen();
			myGoodie()->moveTo(getX(), getY());
		}
	}
}

//ANGRYKLEPTOBOT

AngryKleptoBot::AngryKleptoBot(StudentWorld* world, int startX, int startY) : KleptoBot(world, startX, startY, IID_ANGRY_KLEPTOBOT, 8, 20)
{
}

void AngryKleptoBot::doSomething()
{
	if (!isAlive())
		return;
	if (isItTime())							// checks to see if it is allowed to act
	{
		if (needsClearShot())				// shoot if clear shot towards player
		{
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			getWorld()->addBullet(this);
			return;
		}
		Actor* a = getActorAtLocation(getX(), getY());		
		if (a->isStealable() && myGoodie() == nullptr)			// same as regularkleptobot
		{
			if ((rand() % 10) == 1)
			{
				setGoodie(dynamic_cast<Goodie*>(a));
				a->isStolen();
				getWorld()->playSound(SOUND_ROBOT_MUNCH);
				a->setVisible(false);
				return;
			}
		}
		if (turnsLeft() < distanceBeforeTurning() && moveIfPossible())
		{
			incTurn();
		}
		else
		{
			setNewDistanceBeforeTurning(rand() % 6 + 1);
			resetTurns();

			for (int i = 4; i > 0; i--)
			{
				setRandomDirection(i);
				if (moveIfPossible())
				{
					incTurn();
					break;
				}
			}

		}
	}
}

void AngryKleptoBot::damage(int damageAmt)
{
	Robot::damage(damageAmt);
	if (getHitPoints() <= 0)
	{
		if (myGoodie() != nullptr)					// drop current goodie if have at current location when dead
		{
			myGoodie()->setVisible(true);
			myGoodie()->notStolen();
			myGoodie()->moveTo(getX(), getY());
		}
	}
}

//KLEPTOBOTFACTORY
KleptoBotFactory::KleptoBotFactory(StudentWorld* world, int startX, int startY, ProductType type) : Actor(IID_ROBOT_FACTORY, startX, startY, world)
{
	m_productType = type;
	m_count = 0;
}

void KleptoBotFactory::doSomething()
{
	Actor* a;
	int holder = 0;
	for (int x = getX() - 3; x <= getX() + 3; x++)							// nested for loop to check in 3x3 square
	{
		for (int y = getY() - 3; y <= getY() + 3; y++)
		{
			if ((x >= 0 && x < VIEW_WIDTH) && (y >= 0 && y < VIEW_HEIGHT))		// check for boundaries
			{
				a = getActorAtLocationBackwards(x, y);
				if (a != nullptr)
				{
					if (a->countsInFactoryCensus())								// counts the kleptobots
						holder++;
				}
			}		
		}
	}
	m_count = holder;
	if (m_count < 3)											 // check to see if can add new kleptobot
	{
		a = getActorAtLocationBackwards(getX(), getY());		// check backwards since kleptobots are put at the back of the list
		if (!(a->countsInFactoryCensus()))
		{
			if ((rand() % 50) == 1)								// 1 in 50 chance of spawning kleptobot
			{
				getWorld()->playSound(SOUND_ROBOT_BORN);
				getWorld()->addKleptoBot(this);
			}
		}
	}
}
// WALL

Wall::Wall(int startX, int startY, StudentWorld* world) : Actor(IID_WALL, startX, startY, world)
{
}

// HOLE

Hole::Hole(StudentWorld* world, int startX, int startY) : Actor(IID_HOLE, startX, startY, world)
{
}


// BOULDER

Boulder::Boulder(StudentWorld* world, int startX, int startY) : Actor(IID_BOULDER, startX, startY, world, 10)
{
}

bool Boulder::bePushedBy(Agent* a, int x, int y)
{
	if (a->canPushBoulders() && (getWorld()->canBoulderMoveTo(x, y)))	// checks if player and if boulder can move to location x,y
	{	
		moveTo(x, y);	
		Actor* b = getActorAtLocation(x, y);			// check list from beginning since hole is placed at front of list
		if (b->allowsBoulderColocation())					// deletes boulder and hole
		{
			setDead();
			b->setDead();
		}
		return true;
	}
	return false;
}

// PLAYER

Player::Player(int startX, int startY, StudentWorld* world) : Agent(IID_PLAYER, startX, startY, world, right, 20), m_ammo(20)
{
	restoreHealth();
}

void Player::doSomething()
{
	int key;
	if (getWorld()->getKey(key))		// obtain key from keyboard
	{
		switch (key)
		{
		case KEY_PRESS_DOWN:
			setDirection(down);
			moveIfPossible();
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			moveIfPossible();
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			moveIfPossible();
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			moveIfPossible();
			break;
		case KEY_PRESS_SPACE:
			if (getAmmo() != 0)				// fire if there is ammo
			{
				m_ammo--;
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				getWorld()->addBullet(this);
			}
			break;
		}
	}
}



// EXIT

Exit::Exit(StudentWorld* world, int startX, int startY) : Actor(IID_EXIT, startX, startY, world, none)
{
	setVisible(false);
	makeSoundEffect = true;
}

void Exit::doSomething()
{
	if (!(getWorld()->anyJewels()))				// reveal exit if there are no more jewels
	{
		setVisible(true);
		if (makeSoundEffect)					// create sound effect once
		{
			getWorld()->playSound(SOUND_REVEAL_EXIT);
			makeSoundEffect = false;
		}
	}
	if (!(getWorld()->anyJewels()) && getWorld()->isPlayerColocatedWith(getX(), getY()))		// finish level if player is on exit when its revealed
	{
		getWorld()->setLevelFinished();
	}
}

// PICKUPABLE ITEMS

PickupableItem::PickupableItem(StudentWorld* world, int startX, int startY, int imageID,
	unsigned int score) : Actor(imageID, startX, startY, world)
{
	m_score = score;
}

bool PickupableItem::checkForPlayer()
{
	if (!isAlive())
		return false;
	if (getWorld()->isPlayerColocatedWith(getX(), getY()))	// increase score and die when player lands on it
	{
		getWorld()->increaseScore(score());
		setVisible(false);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		return true;
	}
	return false;
}

// JEWEL

Jewel::Jewel(StudentWorld* world, int startX, int startY) : PickupableItem(world, startX, startY, IID_JEWEL, 50)
{}

void Jewel::doSomething()
{
	if (checkForPlayer())			// increase score, kill jewel, and decrement total number of jewels
		getWorld()->decJewels();
}

// GOODIE 

Goodie::Goodie(StudentWorld* world, int startX, int startY, int imageID, int score) : PickupableItem(world, startX, startY, imageID, score)
{
	m_stolen = false;
}

bool Goodie::isStealable() const
{
	if (m_stolen == true)	
		return false;
	return true;
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, startX, startY, IID_EXTRA_LIFE, 1000)
{}

void ExtraLifeGoodie::doSomething()
{
	if (stolen())			// do nothing if goodie is stolen
		return;
	if (checkForPlayer())		// otherwise increment lives 
		getWorld()->incLives();
}

// RESTOREHEALTHGOODIE

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, startX, startY, IID_RESTORE_HEALTH, 500)
{}

void RestoreHealthGoodie::doSomething()
{
	if (stolen())			// do nothing if goodie is stolen
		return;
	if (checkForPlayer())	// otherwise restore player's health
		getWorld()->restorePlayerHealth();
}

//AMMOGOODIE

AmmoGoodie::AmmoGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, startX, startY, IID_AMMO, 100)
{}

void AmmoGoodie::doSomething()
{
	if (stolen())	// do nothing if goodie is stolen
		return;
	if (checkForPlayer())	// otherwise increase player's ammo
	{
		getWorld()->increaseAmmo();
	}
}