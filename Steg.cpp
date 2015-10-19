#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{ 
	if (hostIn.size() == 0)					// return false if HostIn is empty, leaving hostOut unchanged
		return false;  
	vector<unsigned short> result;			// going to be used to hold the compressed msg
	Compressor::compress(msg, result);	// compresses the msg
	string tabsAndSpaces = BinaryConverter::encode(result);		// encodes the msg to tabs and spaces
	vector<string> preLines;				// vector which holds each line in the hostIn
	vector<string> preLinesEdit;			// gets rid of all the trailing spaces and tabs in each line 
	string holder = "";
	for (int i = 0; i < hostIn.size(); i++)		// separates hostIn into lines
	{
		if (hostIn[i] == '\n' || i == hostIn.size() - 1)
		{
			if (i != 0 && hostIn[i - 1] == '\r')
			{
				holder = holder.substr(0, holder.length() - 1);
			}
			if (i == hostIn.size() - 1 && hostIn[i] != '\n')			// special case where does not end in '\n'
			{
				holder += hostIn[i];
			}
			preLines.push_back(holder);						// put each line into a vector not including '\n'
			holder = "";
			continue;
		}
		
		holder += hostIn[i];
	}
	
	int counter = 0;
	for (int k = 0; k < preLines.size(); k++)	// goes through the vector to remove trailing tabs and spaces
	{
		holder = "";
		string line = preLines[k]; 
		counter = line.size();
		for (int l = line.size() - 1; l > 0; l--)
		{
			if (line[l] == '\n' || line[l] == '\t' || line[l] == '\r' || line[l] == ' ')	// ignore new lines, spaces, tabs, and returns
			{
				counter--;
			}
			else
				break;
		}
		for (int j = 0; j < counter; j++)
		{
			holder += line[j];
		}
		preLinesEdit.push_back(holder);
	}

	int numLines = preLinesEdit.size();			// N
	int lengthCompressingEncoding = tabsAndSpaces.size();		// L
	int firstHalfSubstrings = lengthCompressingEncoding % numLines;		// L % N
	int firstHalfSubstringLength = (lengthCompressingEncoding / numLines) + 1; // (L/N) + 1
	int secondHalfSubstringLength = lengthCompressingEncoding / numLines;	// L/N

	int currentSpaceOrTab = 0;
	for (int i = 0; i < numLines; i++)
	{
		if (i < firstHalfSubstrings)
		{
			for (int k = 0; k < firstHalfSubstringLength; k++)
			{
				preLinesEdit[i] += tabsAndSpaces[currentSpaceOrTab];
				currentSpaceOrTab++;
			}
		}
		else 
		{
			for (int l = 0; l < secondHalfSubstringLength; l++)
			{
				preLinesEdit[i] += tabsAndSpaces[currentSpaceOrTab];
				currentSpaceOrTab++;
			}
		}
	}

	hostOut = "";
	for (int p = 0; p < preLinesEdit.size(); p++)				// adds the edited lines with the appended tabs and spaces into hostOut
	{
		hostOut += preLinesEdit[p];
		hostOut += '\n';
	}
	
	return true;
}

bool Steg::reveal(const string& hostIn, string& msg)
{
	vector<string> lines;
	string holder;
	string result;
	for (int i = 0; i < hostIn.size(); i++)						// separates hostIn into lines
	{
		if (hostIn[i] == '\n')
		{
			lines.push_back(holder);
			holder = "";
			continue;
		}
		holder += hostIn[i];
	}
	int counter;
	int tabSpaceCounter = 0;
	vector<string> encodedMsg;
	for (int k = 0; k < lines.size(); k++)						//goes through each line
	{
		holder = "";
		string line = lines[k];
		counter = line.size();
		for (int l = line.size() - 1; l >= 0; l--)				// counts the tabs and spaces and then changes the lines to just the tabs and spaces
		{
			if (line[l] == '\t' || line[l] == ' ')
			{
				tabSpaceCounter++;
			}
			else
			{
				lines[k] = lines[k].substr(l + 1);
				break;
			}

		}
	}
	if (tabSpaceCounter % 16 != 0)								// needs to check it has proper amount of tabs and spaces
		return false;
	for (int z = 0; z < lines.size(); z++)
	{
		result += lines[z];										// combines the tabs and spaces all together
	}
	vector<unsigned short> answer;
	if (BinaryConverter::decode(result, answer))			// decodes and decompresses to get msg
	{
		Compressor::decompress(answer, msg);
	}
	return true;  
}
