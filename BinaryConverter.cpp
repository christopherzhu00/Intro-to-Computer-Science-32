#include "provided.h"
#include <string>
#include <vector>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
	string holder = "";
	string answer = "";
	for (int i = 0; i < numbers.size(); i++)		// converts unsigned shorts to binary string
	{
		holder += convertNumberToBitString(numbers[i]);
	}
	for (int k = 0; k < holder.size(); k++)	// converts binary to spaces and tabs
	{
		if (holder[k] == '0')
		{
			answer += ' ';
		}
		else
			answer += '\t';
	}
	return answer;  
}

bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers)
{
	if (bitString.size() % 16 != 0)	// not a proper binary string
		return false;  
	for (int i = 0; i < bitString.size(); i++)	// checks to see if every char is a space or tab
	{
		if (bitString[i] != ' ' && bitString[i] != '\t')
			return false;
	}
	numbers.clear();		// clears numbers vector
	unsigned short value;
	string result;
	for (int k = 0; k < bitString.size() / 16; k++)		// goes through amount of letters
	{
		string holder = "";								// used to hold each binary string
		for (int l = 0; l < 16; l++)					// combines 0's and 1's into a 16 length string
		{
			holder += bitString[l + (k * 16)];
		}
		for (int z = 0; z < holder.size(); z++)
		{
			if (holder[z] == ' ')
				result += '0';
			else
				result += '1';
		}
		if (convertBitStringToNumber(result, value))	// converts binary string and stores value
		{
			numbers.push_back(value);					// puts the unsigned short into the vector
		}
		result = "";
	}
	return true;						//successful conversion
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}
