#include "provided.h"
#include <string>
#include "http.h"
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
	string page; // to hold the contents of the web page
	// The next line downloads a web page for you. So easy!
	string result;
	if (HTTP().get(url, page))
	{
		if (Steg::hide(page, msg, result))
		{
			host = result;
			return true;
		}
	}
	return false;  // This compiles, but may not be correct
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
	string page;
	if (HTTP().get(url, page))
	{
		if (Steg::reveal(page, msg))
			return true;
	}
	return false;  // This compiles, but may not be correct
}
