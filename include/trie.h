#ifndef __TRIE_NODE_H__
#define __TRIE_NODE_H__

#include <CVector.h>

template <typename T, int Base>
class Trie
{
	Trie* childs[Base];
	T n;

	unsigned int val;

	void init(unsigned int val);

	Trie(int mult);

	T& create(unsigned int number, int mult);

	CVector<int> get_keys(int val);

	Trie& move(Trie& other);

public:

	~Trie();

	Trie();

	Trie(const Trie& other);

	Trie& operator= (Trie other);

	T& operator[](unsigned int number);

	bool is_empty();

	CVector<int> get_keys();
};

template <typename T>
class Trie10 : public Trie < T, 10 > {};

template <typename T>
class Trie5 : public Trie < T, 5 > {};

template <typename T>
class Trie2 : public Trie < T, 2 > {};

#include <trie.cpp>

#endif