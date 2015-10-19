#include <iostream>
using namespace std;

#include "History.h"

History::History(int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;
	for (int i = 0; i < nRows; i++)
	{
		for (int k = 0; k < nCols; k++)						// fill array with no jumps
		{
			m_jumps[i][k] = 0;
		}
	}
}

bool History::record(int r, int c)
{
	if (r < 1 || c < 1 || r > m_rows || c > m_cols)			// check to see if within bounds
	{
		return false;
	}
	m_jumps[r-1][c-1]++;									// increment number of jump for the grid location 
	return true;
}

void History::display() const
{
	char grid[MAXROWS][MAXCOLS];							// create a grid of characters
	int r, c;
	for (r = 0; r < m_rows; r++)							// fill grid with '.'s
	{
		for (c = 0; c < m_cols; c++)
		{
			grid[r][c] = '.';
		}
	}
	for (int i = 0; i < m_rows; i++)
	{
		for (int k = 0; k < m_cols; k++)
		{
			int jumps = m_jumps[i][k];						// find out how many jumps in the grid location
			if (jumps >= 26)								// if 26 or more successes, make the char at the location a 'Z'
			{
				grid[i][k] = 'Z';
			}
			else if (jumps > 0)								// otherwise just increment by the number of jumps
			{
				grid[i][k] = 'A' + (jumps - 1);
			}
		}
	}
	clearScreen();											// clear the screen
	for (r = 0; r < m_rows; r++)							// prints out the screen
	{
		for (c = 0; c < m_cols; c++)
		{
			cout << grid[r][c];
		}
		cout << endl;
	}
	cout << endl;											// adds a new line at the end

}