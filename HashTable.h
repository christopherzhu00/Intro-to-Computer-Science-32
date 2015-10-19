#ifndef HASHTABLE_INCLUDED
#define HASHTABLE_INCLUDED

template <typename KeyType, typename ValueType>
class HashTable
{
public:
	HashTable(unsigned int numBuckets, unsigned int capacity);
	~HashTable();
	bool isFull()	const;
	bool set(const KeyType&	key, const ValueType& value, bool permanent = false);
	bool get(const KeyType& key, ValueType& value)	const;
	bool touch(const KeyType& key);
	bool discard(KeyType& key, ValueType& value);
private:
	//	We	prevent a	HashTable from	being	copied	or	assigned	by	declaring	the
	//	copy	constructor	and	assignment	operator	private	and	not	implementing	them.
	HashTable(const HashTable&);
	HashTable& operator=(const HashTable&);
	Node** m_buckets;
	unsigned int m_capacity;
	unsigned int m_currentCapacity;
	unsigned int m_numBuckets;
	Node* m_head;
	Node* m_tail;
	struct Node
	{
		KeyType m_key;
		ValueType m_value;
		Node* next;
		Node* older;
		Node* newer;
		bool isPermanent;
	};
};

template <typename KeyType, typename ValueType>
HashTable::HashTable(unsigned int numBuckets, unsigned int capacity)
{
	m_nodes = new Node*[numBuckets];
	for (int i = 0; i < numBuckets; i++)
	{
		m_nodes[i] = nullptr;
	}
	m_numBuckets = numBuckets;
	m_capacity = capacity;
	m_head = nullptr;
	m_tail = nullptr;
}

template <typename KeyType, typename ValueType>
HashTable::~HashTable()
{
	for (int i = 0; i < m_numBuckets; i++)
	{
		if (m_buckets[i].value)
	}
}
template <typename KeyType, typename ValueType>
bool HashTable::isFull() const
{
	return m_currentCapacity == m_capacity;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::set(const KeyType& key, const ValueType& value, bool permanent = false)
{
	unsigned int computeHash(KeyType); // prototype	
	unsigned int result = computeHash(key) % m_numBuckets;
	if (isFull())
		return false;
	Node* holder = m_buckets[result];
	bool found = false;
	while (holder->next != nullptr)
	{
		if (holder->value != value)
			holder = holder->next;
		else
		{
			found = true;
			break;
		}
	}
	if (!found)
	{
		Node* p = new Node;
		p->m_key = key;
		p->m_value = value;
		p->isPermanent = permanent;
		holder->next = p;
		if (p->isPermanent == false)
		{
			touch(key);
		}

	}
	else
	{
		holder->m_value = value;
		if (holder->isPermanent == false)
		{
			touch(key);
		}
	}
	return true;
	
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::touch(const KeyType& key)
{
	unsigned int computeHash(KeyType); // prototype	
	unsigned int result = computeHash(key) % m_numBuckets;
	Node* holder = m_buckets[result];
	if (holder->isPermanent == false)
	{
		if (m_head == nullptr && m_tail == nullptr)
		{
			m_head = holder;
			m_tail = holder;
		}
		Node* temp = m_head->older;
		m_head = holder;
		holder->older = temp;
		temp->newer = holder;
		return true;
	}
	return false;
}

#endif // HASHTABLE_INCLUDED