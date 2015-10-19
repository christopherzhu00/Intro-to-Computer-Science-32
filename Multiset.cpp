#include "Multiset.h"

Multiset::Multiset()													
	:m_uniqueSize(0), m_size(0), head(nullptr)
{}

Multiset::~Multiset()
{
	Node *p = head;							// go through all the Nodes and delete them to free memory
	while (p != nullptr)
	{
		Node *next = p->next;
		delete p;
		p = next;
	}
}

Multiset::Multiset(const Multiset& other)				// copy constructor
{
	m_size = other.m_size;							// copy values
	m_uniqueSize = other.m_uniqueSize;
	head = nullptr;
	Node *p = other.head;
	while (p != nullptr)						// allocate new memory for every Node that is in other to make a copy
	{
		Node *copy = new Node;
		copy->m_count = p->m_count;
		copy->m_value = p->m_value;
		copy->next = head;
		if (head != nullptr)					// connect the previous node to the new node
			head->prev = copy;
		copy->prev = nullptr;
		head = copy;							// make the new value the head
		p = p->next;
	}
}

Multiset& Multiset::operator=(const Multiset& other)		// assignment operator
{
	if (&other == this)							// check for alias
		return (*this);
	Node *q = head;
	while (q != nullptr)						// delete other since we want to make copy
	{
		Node *next = q->next;
		delete q;
		q = next;
	}
	m_size = other.m_size;						// start copying data
	m_uniqueSize = other.m_uniqueSize;
	head = nullptr;
	Node *p = other.head;
	while (p != nullptr)						// allocate new memory for the copies of the nodes in other
	{
		Node *copy = new Node;
		copy->m_count = p->m_count;
		copy->m_value = p->m_value;
		copy->next = head;
		if (head != nullptr)					// connect previous node with new node
			head->prev = copy;
		copy->prev = nullptr;
		head = copy;
		p = p->next;							// traverse through all the nodes
	}
	return *this;
}

bool Multiset::empty() const
{
	return size() == 0;
}

int Multiset::size() const
{
	return m_size;
}

int Multiset::uniqueSize() const
{
	return m_uniqueSize;
}

bool Multiset::insert(const ItemType& value)
{
	Node *p = head; 
	bool shouldAdd = true;
	while (p != nullptr)					// traverse to see if value is already in the Multiset
	{
		if (p->m_value == value)
		{
			p->m_count++;
			shouldAdd = false;
			break;							// break out of loop if found
		}
		p = p->next;
	}
	if (shouldAdd)							// add a new Node to the beginning of the linked list
	{
		Node *h = new Node;
		h->m_value = value;
		h->m_count = 1;
		h->next = head;
		if (head != nullptr)				// check for special case when multiset is empty
			head->prev = h;
		h->prev = nullptr;
		head = h;
		m_uniqueSize++;
	}
	m_size++;							// if we insert a node the size always increases
	return true;
}

int Multiset::erase(const ItemType& value)
{
	if (head == nullptr)					// if empty multiset just return
	{
		return 0;
	}
	if (head->m_value == value)				// check for first item in Multiset
	{
		if (head->m_count >= 1)				// subtract 1 from count if there is 1 or more rather than delete
		{
			head->m_count--;
			return 1;
		}
		else
		{
			Node *killMe = head;			// remove from multiset entirely
			head = killMe->next;
			if (killMe->next != nullptr)			// check for if there is only 1 item
			{
				(killMe->next)->prev = nullptr;
			}
			delete killMe;
			return 1;
		}
	}
	Node *p = head;
	while (p != nullptr)		// traverse array to find node above
	{
		if (p->next != nullptr && p->next->m_value == value)
			break;
		p = p->next;
	}
	if (p != nullptr)				// found value
	{ 
		if ((p->next)->m_count >= 1)			// subtracts 1 from count if there is 1 or more
		{
			(p->next)->m_count--;
			return 1;
		}
		else
		{
			Node *killMe = p->next;
			p->next = killMe->next;
			if (killMe->next != nullptr)			// links the previous node to the node after if applicable
			{
				(killMe->next)->prev = p;
			}
			delete killMe;
			return 1;
		}
	}
	return 0;				// erased nothing so return 0
}

int Multiset::eraseAll(const ItemType& value)
{
	if (head == nullptr)						// return 0 if empty
		return 0;
	Node *p = head;
	if (head->m_value == value)			// check special case where first node is the value we want to erase
	{
		Node* killMe = head;
		head = killMe->next;
		int amount = killMe->m_count;
		delete killMe;
		m_uniqueSize--;					
		m_size -= amount;						// make sure to change size accordingly
		return amount;
	}
	while (p != nullptr)		// traverse array to find node above
	{
		if (p->next != nullptr && p->next->m_value == value)
			break;
		p = p->next;
	}
	if (p != nullptr)				// found value
	{
		Node *killMe = p->next;
		p->next = killMe->next;
		if (killMe->next != nullptr)			// links the previous node to the node after if applicable
		{
			(killMe->next)->prev = p;
		}
		int amount = killMe->m_count;
		delete killMe;
		m_uniqueSize--;
		m_size -= amount;						// make sure to change size accordingly
		return amount;
	}
	return 0;				// erased nothing so return 0
}

bool Multiset::contains(const ItemType& value) const
{
	Node *p = head;
	while (p != nullptr)					// traverse the multilist
	{
		if (p->m_value == value)			// found the value in the multilist
		{
			return true;
		}
		p = p->next;
	}
	return false;							// not found
}

int Multiset::count(const ItemType& value) const
{
	Node *p = head;
	while (p != nullptr)					// traverse the multilist
	{
		if (p->m_value == value)			// returns the count if found
		{
			return p->m_count;
		}
		p = p->next;
	}
	return 0;								// not found in multilist
}

int Multiset::get(int i, ItemType& value) const
{
	if (i < 0 || i >= m_uniqueSize)				// return 0 if doesn't satisfy preconditions
		return 0;
	int counter = 0;
	Node *p = head;
	while (p->next != nullptr && counter < i)		// want to get the i'th node 
	{
		p = p->next;
		counter++;
	}

	value = p->m_value;								
	return p->m_count;
}

void Multiset::swap(Multiset& other)
{
	Node *placeHolderStart = other.head;				// create placeholder values to allow for swapping
	int holderSize = other.m_size;			
	int holderUniqueSize = other.m_uniqueSize;
	other.head = head;									// exchange private values
	head = placeHolderStart;
	other.m_size = m_size;
	other.m_uniqueSize = m_uniqueSize;
	m_size = holderSize;
	m_uniqueSize = holderUniqueSize;
}

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	ItemType current;
	int holder;
	int counter;
	if ((&ms1 == &result) && (&ms2 == &result))					// check for aliasing ms1, ms2, and result are the same
	{
		for (int i = 0; i < result.uniqueSize(); i++)
		{
			holder = ms1.get(i, current);
			for (counter = 0; counter < holder; counter++)				// double the values
			{
				result.insert(current);
			}
		}
		return;
	}
	if (&ms1 != &result && &ms2 != &result)			// check for aliasing
	{
		int deleted;
		int total = result.uniqueSize();
		for (int i = 0; i < total; i++)			// empty the multiset
		{
			holder = result.get(0, current);		// delete head and move head until deleted everything
			deleted = result.eraseAll(current);
		}
	}
	if (&ms1 != &result)											// checks for aliasing with ms1
	{
		for (int k = 0; k < ms1.uniqueSize(); k++)					// inserts the values of ms1 into result
		{
			holder = ms1.get(k, current);
			for (counter = 0; counter < holder; counter++)
			{
				result.insert(current);
			}
		}
	}
	if (&ms2 != &result)											// checks for aliasing with ms2
	{
		for (int j = 0; j < ms2.uniqueSize(); j++)					// inserts the values of ms2 into result
		{
			holder = ms2.get(j, current);
			for (counter = 0; counter < holder; counter++)
			{
				result.insert(current);
			}
		}
	}
}

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
	ItemType current;
	int holder;
	int otherHolder;
	if ((&ms1 == &result) && (&ms2 == &result))		// empty multiset since n2-n1 = 0 due to aliasing
	{
		int deleted;
		int total = result.uniqueSize();
		for (int i = 0; i < total; i++)			// empty the multiset
		{
			holder = result.get(0, current);		// delete head and move head until deleted everything
			deleted = result.eraseAll(current);
		}
		return;
	}
	if (&ms1 != &result && &ms2 != &result)			// check for aliasing for both ms1 and ms2 
	{
		int deleted;
		int total = result.uniqueSize();
		for (int i = 0; i < total; i++)			// empty the multiset
		{
			holder = result.get(0, current);		// delete head and move head until deleted everything
			deleted = result.eraseAll(current);
		}
	}
	int counter;
	if (&ms1 == &result)											// checks for aliasing with ms1
	{
		for (int k = 0; k < ms1.uniqueSize(); k++)					// traversing ms1
		{
			holder = ms1.get(k, current);
			if (ms2.contains(current))								// checks to see if other multilist contains the variable to find n1-n2
			{
				if (holder - ms2.count(current) > 0)				// do not consider the value if n1-n2 < 0, otherwise n1-n2 for total value
				{
					holder -= ms2.count(current);
					for (counter = 0; counter <= holder; counter++)
					{
						result.erase(current);
					}
				}
				else
				{
					int deleted = result.eraseAll(current);			// delete the node if negative count after n1-n2
					k--;
					continue;
				}
			}
		}
	}
	else if (&ms2 == &result)										// check aliasing ms2 and result
	{
		for (int i = 0; i < ms2.uniqueSize(); i++)
		{
			holder = ms2.get(i, current);
			if (holder == 0)
				break;
			if (!ms1.contains(current))
			{
				int deleted = result.eraseAll(current);
				i--;
			}
		}
		for (int k = 0; k < ms1.uniqueSize(); k++)					// traversing ms1
		{
			holder = ms1.get(k, current);
			if (ms2.contains(current))								// checks to see if other multilist contains the variable to find n1-n2
			{
				if (holder - ms2.count(current) > 0)				// do not consider the value if n1-n2 < 0, otherwise n1-n2 for total value
				{
					holder -= ms2.count(current);
				}
				else
				{
					int deleted = result.eraseAll(current);
					continue;
				}
			}
			for (counter = 0; counter < holder; counter++)
			{
				result.insert(current);
			}
		}
	}
	else
	{
		for (int k = 0; k < ms1.uniqueSize(); k++)					// traversing ms1
		{
			holder = ms1.get(k, current);
			if (ms2.contains(current))								// checks to see if other multilist contains the variable to find n1-n2
			{
				if (holder - ms2.count(current) >= 0)				// do not consider the value if n1-n2 < 0, otherwise n1-n2 for total value
				{
					holder -= ms2.count(current);
				}
			}
			for (counter = 0; counter < holder; counter++)			// insert the value n1-n2 times assuming n1 > n2
			{
				result.insert(current);
			}
		}
	}

}
