/*#include "Multiset.h"

#include <iostream>
#include <cassert>
using namespace std;*/
#include "Multiset.h"
#include <iostream>
#include <cassert>
using namespace std;


int main()
{
	Multiset ms;
	assert(ms.empty());
	unsigned long x = 999;
	assert(ms.get(0, x) == 0 && x == 999);  // x unchanged by get failure
	assert(!ms.contains(42));
	ms.insert(42);
	ms.insert(42);
	ms.insert(10);
	ms.insert(5);
	Multiset ab;
	ab.insert(3);
	ab.insert(5);
	Multiset cd;
	cd.insert(4);
	cd.insert(10);
	cd.insert(29);
	cd.insert(29);
	ab.swap(ab);
	cout << ab.size() << endl;
	cout << ab.uniqueSize() << endl;
	
	cout << "Passed all tests" << endl;

}