#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED
class Pit;	 // This is needed to let the compiler know that Pit is a
// type name, since it's mentioned in the Player declaration.

class Player
{
public:
	// Constructor
	Player(Pit *pp, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	int  age() const;
	bool isDead() const;

	// Mutators
	void   stand();
	void   move(int dir);
	void   setDead();

private:
	Pit*  m_pit;
	int   m_row;
	int   m_col;
	int   m_age;
	bool  m_dead;
};

#endif // PLAYER_INCLDUED