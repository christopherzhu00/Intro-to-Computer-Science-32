#ifndef MULTISET_INCLUDED
#define MULTISET_INCLUDED

#include <string>
typedef std::string ItemType;


class Multiset
{
public:
	Multiset();    // Create an empty multiset.

	~Multiset();									// Housekeeping functions
	Multiset(const Multiset& other);			
	Multiset& operator=(const Multiset& rhs);

	bool empty() const;  // Return true if the multiset is empty, otherwise false.

	int size() const;
	// Return the number of items in the multiset.  For example, the size
	// of a multiset containing "cumin", "cumin", "cumin", "turmeric" is 4.

	int uniqueSize() const;
	// Return the number of distinct items in the multiset.  For example,
	// the uniqueSize of a multiset containing "cumin", "cumin", "cumin",
	// "turmeric" is 2.

	bool insert(const ItemType& value);
	// Insert value into the multiset.  Return true if the value was
	// actually inserted.  Return false if the value was not inserted
	// (perhaps because the multiset has a fixed capacity and is full).

	int erase(const ItemType& value);
	// Remove one instance of value from the multiset if present.
	// Return the number of instances removed, which will be 1 or 0.

	int eraseAll(const ItemType& value);
	// Remove all instances of value from the multiset if present.
	// Return the number of instances removed.

	bool contains(const ItemType& value) const;
	// Return true if the value is in the multiset, otherwise false.

	int count(const ItemType& value) const;
	// Return the number of instances of value in the multiset.

	int get(int i, ItemType& value) const;
	// If 0 <= i < uniqueSize(), copy into value an item in the
	// multiset and return the number of instances of that item in
	// the multiset.  Otherwise, leave value unchanged and return 0.
	// (See below for details about this function.)

	void swap(Multiset& other);
	// Exchange the contents of this multiset with the other one.

private:
	struct Node										// struct Node only used by Multisets
	{
		ItemType m_value;							// holds the value
		int m_count;								// holds the amount 
		Node *next, *prev;							// points to the next node and prev node
	};
	Node *head;										// pointer to the first node
	int m_uniqueSize;
	int m_size;
};


void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result);
void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result);

#endif // MULTISET_INCLUDED