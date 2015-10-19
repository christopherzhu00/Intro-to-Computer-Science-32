// We have not yet produced the test driver main routine for you.

#include "provided.h"
#include <iostream>
#include "http.h"
using namespace std;

unsigned int hashFunc(string &name)
{
	int i, total = 0;

	for (i = 0; i < name.length(); i++)
	{
		total += i;
		total = (total + 2) * name[i];
	}
	return total;
}
int main()
{

	string pageText =
		"<html>\n"
		"<head>\n"
		" <title>My Pretend Web Page</title>\n"
		"</head>\n"
		"<body>\n"
		"<h2>My Pretend Web Page<h1>\n"
		"<p>\n"
		"I wish I were creative enough to have something interesting \n"
		"to say here.  I'm not, though, so this is a boring page.\n"
		"</p>\n"
		"<p>\n"
		"Oh, well.\n"
		"</p>\n"
		"</body>\n"
		"</html>\n"
		;
	HTTP().set("http://boring.com", pageText);
	string plan =
		"Lefty and Mugsy enter the bank by the back door.\n"
		"Shorty waits out front in the getaway car.\n"
		"Don't let the teller trip the silent alarm.\n"
		;
	if (!WebSteg::hideMessageInPage("http://boring.com", plan, pageText))
	{
		cout << "Error hiding!" << endl;
		return 1;
	}
	HTTP().set("http://boring.com", pageText);
	string msg;
	if (!WebSteg::revealMessageInPage("http://boring.com", msg))
	{
		cout << "Error revealing!" << endl;
		return 1;
	}
	if (msg != plan)
	{
		cout << "Message not recovered properly:\n" << msg;
		return 1;
	}
	cout << "Recovered text is what was hidden:\n" << msg;
/*	cout << "Test driver not yet written." << endl;

	
	 //string text;
	 //WebSteg::hideMessageInPage("http://cs.ucla.edu", "Hello there!", text);

	 string result;
	 string page;
	 string msg;
//	 HTTP().get("http://cs.ucla.edu", page);
//	 Steg::hide("asdfasdf\nasdf\n", "~~~~~~~~~~~~", result);
//	 cout << result << endl;
//	 Steg::reveal(result, msg);
//	 cout << msg << endl;

	/* for (int i = 0; i < page.length() - 1; i++)
	 {
		 if (page[i] == '\r' && page[i + 1] == '\n')
		//	 cout << "blah";
	 }
	 string text;


	 HTTP().set("http://a.com", "Hi\npoop\n123123\n123");

	 if ( ! WebSteg::hideMessageInPage("http://a.com", "Hello there!", text))
		cout << "Error hiding!" << endl;

	 cout << text << endl;
//	
	 Steg::hide("Hi\npoop\n123123\n123", "~~~~~~~~~~~~", result);
	 Steg::reveal(result, msg);
	 cout << msg << endl;

	 HTTP().set("http://a.com", text);
	 if ( ! WebSteg::revealMessageInPage("http://a.com", msg)  )
	 	cout << "Error revealing!" << endl;*/

	 
}
