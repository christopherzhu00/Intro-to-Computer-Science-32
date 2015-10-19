#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>
using namespace std;
//AJKSLFJAL:SKFJ:SAJFL:ASJF:LJA:LSJFAKLSJF:LKAJSF:ASJLFJAL:SFJAL:SJF:ASJFASJKF
// ERIC IS POOP
void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
	int smaller;
	if (s.size() / 2 + 512 > 16384)
		smaller = 16384;
	else
		smaller = s.size() / 2 + 512;
	HashTable<string, unsigned short> h(smaller * 2, smaller);
	string a = "";
	for (int i = 0; i < 256; i++)
	{
		a = "";
		a += static_cast<char>(i);
		h.set(a, i, true);
	}
	int nextFreeID = 256;
	unsigned short garbage;
	string runSoFar = "";
	unsigned short x;
	unsigned short cv;
	vector<unsigned short> result;
	unsigned short discardValue;
	string discardString;
	for (int k = 0; k < s.size(); k++)
	{
		string expandedRun = runSoFar + s[k];
		if (h.get(expandedRun, garbage))
		{
			runSoFar = expandedRun;
			continue;
		}
		h.get(runSoFar, x);						// CANCER HELP
		result.push_back(x);
		h.touch(runSoFar);
		runSoFar = "";
		string c = "";
		c += s[k];
		h.get(c, cv);
		result.push_back(cv);
		
		if (!(h.isFull()))
		{
			h.set(expandedRun, nextFreeID, true);
			nextFreeID++;
		}
		else
		{
			h.discard(discardString, discardValue);
			h.set(expandedRun, discardValue, true);
		}
	}
	if (runSoFar != "")
	{
		h.get(runSoFar, x);
		result.push_back(x);
	}
	result.push_back(smaller);
	numbers = result;
	  // This compiles, but may not be correct
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
	if (numbers.size() == 0)
		return false;
	int smaller;
	s = "";
	unsigned short capacity = numbers[numbers.size() - 1];
/*	if (s.size() / 2 + 512 > 16384)
		smaller = 16384;
	else
		smaller = s.size() / 2 + 512;*/
	HashTable<unsigned short, string> h(capacity * 2, capacity);
	string a = "";
	for (int i = 0; i < 256; i++)
	{
		a = "";
		a += static_cast<char>(i);
		h.set(i, a, true);
	}
	int nextFreeID = 256;
	string runSoFar = "";
	string add;
	unsigned short discardValue;
	string discardString;
	string garbage;
	unsigned short us;
	for (int i = 0; i < numbers.size(); i++)
	{
		us = numbers[i];
		if (us <= 255)
		{
			add = "";
			h.get(us, add);
			s += add;
			if (runSoFar == "")
			{
				runSoFar = add;
				continue;
			}
			else
			{
//				runSoFar += add;
				string expandedRun = runSoFar + add;
				if (!(h.isFull()))
				{
					h.set(nextFreeID, expandedRun);
					nextFreeID++;
				}
				else
				{
					h.discard(discardValue, discardString);
					h.set(discardValue, expandedRun);
				}
				runSoFar = "";
				continue;
			}
		}
		else
		{
			if (!(h.get(us, garbage)))
				return false;
			h.touch(us);
			s += garbage;
			runSoFar = garbage;
		}
	}
	return true;  // This compiles, but may not be correct
}
