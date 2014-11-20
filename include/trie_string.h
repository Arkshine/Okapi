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

#ifndef __TRIE_STRING_H__
#define __TRIE_STRING_H__

#include <CVector.h>
#include <string_.h>

const int Base = 4;
const int Mult = 4;

template <typename T>
struct TrieString
{
	public:

		TrieString* childs[Base];
		T n;

		char val;

		void init(char val);

		TrieString(char val);

		T& create(char* s);

		CVector<String> get_keys(String s);

		TrieString& move(TrieString& other);

	public:

		~TrieString();

		TrieString();

		TrieString(const TrieString& other);

		TrieString& operator= (TrieString other);

		T& operator[](char* s);

		bool is_empty();

		CVector<String> get_keys();
};

#include <trie_string.cpp>

#endif // __TRIE_STRING_H__
