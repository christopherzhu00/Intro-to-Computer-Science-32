#ifndef SNAKE_INCLUDED
#define SNAKE_INCLUDED

class Pit;  // This is needed to let the compiler know that Pit is a
// type name, since it's mentioned in the Snake declaration.

class Snake
{
public:
	// Constructor
	Snake(Pit* pp, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;

	// Mutators
	void move();

private:
	Pit* m_pit;
	int  m_row;
	int  m_col;
};

#endif //SNAKE_INCLUDED
