#ifndef HISTORY_INCLUDED
#define HISTORY_INCLUDED

#include "globals.h"
class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int m_rows;
	int m_cols; 
	int m_jumps[MAXROWS][MAXCOLS];				// create a table to show how many jumps a player has done
	
};

#endif // HISTORY_INCLUDED