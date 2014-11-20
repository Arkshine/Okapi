//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Okapi Module
//

#ifndef __TRIE_NODE_H__
#define __TRIE_NODE_H__

#include <CVector.h>

template <typename T, int Base>
class Trie
{
	private:

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

#endif // __TRIE_NODE_H__
